#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<time.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>
//#include<iostream>
//using namespace std;
#define MAX_EVENTS 500
const int kTimeOut = -1;
struct myevent_s
{
    int fd;
    void(*call_back)(int fd, int events, void* arg);
    int events;
    void *arg;
    int status;
    char buff[128];
    int len;
    long last_active;
};

//set event
void EventSet(myevent_s *ev, int fd, void(*call_back)(int, int, void*), void* arg)
{
    ev->fd = fd;
    ev->call_back = call_back;
    ev->events = 0;
    ev->arg = arg;
    ev->status = 0;
    ev->last_active = time(NULL);
}

//add/mod an event to epoll
void EventAdd(int epollFd, int events, myevent_s *ev)
{
    struct epoll_event epv = {0, {0}};
    int op;
    epv.data.ptr = ev;
    epv.events = ev->events = events;
    if (ev->status == 1) {
        op = EPOLL_CTL_MOD;
    } else {
        op = EPOLL_CTL_ADD;
	ev->status = 1;
    }
    if (epoll_ctl(epollFd, op, ev->fd, &epv) < 0) {
        printf("Event Add faild[fd = %d]\n", ev->fd);
    } else {
        printf("Event Add OK[fd=%d]\n", ev->fd);
    }
}

//delete an event from epoll
void EventDel(int epollFd, myevent_s *ev)
{
   struct epoll_event epv = {0, {0}};
   if (ev->status != 1) return ;
   epv.data.ptr = ev;
   ev->status = 0;
   epoll_ctl(epollFd, EPOLL_CTL_DEL, ev->fd, &epv);
}

int g_epollFd;

myevent_s g_Events[MAX_EVENTS + 1];

void RecvData(int fd, int events, void *arg);
void SendData(int fd, int events, void *arg);

void AcceptConn(int fd, int events, void *arg)
{
    struct sockaddr_in sin;
    socklen_t len = sizeof(struct sockaddr_in);
    int nfd, i;

    if (nfd = accept(fd, (struct sockaddr*)&sin, &len) == -1) {
        if (errno != EAGAIN && errno != EINTR) {
	    printf("%s:bad accept", __func__);
	}

	return;
    }
    do {
        for (i = 0; i < MAX_EVENTS; ++i) {
	    if (g_Events[i].status == 0) {
	        break;
	    }
	}
	if (i == MAX_EVENTS) {
	    printf("%s:max connection limit[%d].", __func__, MAX_EVENTS);
	    break;
	}
	if (fcntl(nfd, F_SETFL, O_NONBLOCK < 0)) break;

	EventSet(&g_Events[i], nfd, RecvData, &g_Events[i]);
	EventAdd(g_epollFd, EPOLLIN | EPOLLET, &g_Events[i]);

	printf("new conn[%s:%d][time:%d]\n", inet_ntoa(sin.sin_addr), ntohs(sin.sin_port), g_Events[i].last_active);
    }while(0);
}

void RecvData(int fd, int events, void *arg)
{
    struct myevent_s *ev = (struct myevent_s*) arg;
    int len;
    len = recv(fd, ev->buff, sizeof(ev->buff) - 1, 0);
    EventDel(g_epollFd, ev);
    if (len > 0) {
        ev->len = len;
	ev->buff[len] = '\0';
	printf("C[%d]:%s\n", fd, ev->buff);

	EventSet(ev, fd, SendData, ev);
	EventAdd(g_epollFd, EPOLLOUT |  EPOLLET, ev);
    } else if (len == 0) {
        close(ev->fd);
	printf("[fd = %d]closed gracefully.\n", fd);
    } else {
        close(ev->fd);
//	printf("recv[fd = %d] error [%d]:%s\n", fd, errno, strerror(errno));
        
    }
}


void SendData(int fd, int events, void *arg)
{
    struct myevent_s *ev = (struct myevent_s*)arg;
    int len;
    len = send(fd, ev->buff, ev->len, 0);
    ev->len = 0;
    EventDel(g_epollFd, ev);
    if (len > 0) {
        EventSet(ev, fd, RecvData, ev);
	EventAdd(g_epollFd, EPOLLIN | EPOLLET, ev);
    } else {
        close(ev->fd);
	printf("recv[fd=%d]error[%d]\n", fd, errno);
    }
}


void InitListenSocket(int epollFd, short port)
{
    int listenFd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(listenFd, F_SETFL, O_NONBLOCK);
    printf("server listen fd = %d\n", listenFd);
    EventSet(&g_Events[MAX_EVENTS], listenFd, AcceptConn, &g_Events[MAX_EVENTS]);
    EventAdd(epollFd, EPOLLIN|EPOLLET, &g_Events[MAX_EVENTS]);

    sockaddr_in sin;
    bzero(&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);
    bind(listenFd, (const sockaddr*)&sin, sizeof(sin));
    listen(listenFd, 5);
    printf("listen over!\n");
}
int main(int argc, char **argv)
{
    short port = 12345;
    if (argc == 2) {
        port = atoi(argv[1]);
    }

    g_epollFd = epoll_create(MAX_EVENTS);
    if (g_epollFd <= 0) printf("create epoll failed.%d\n", g_epollFd);
    InitListenSocket(g_epollFd, port);
    struct epoll_event events[MAX_EVENTS];
    printf("server running:port[%d]\n", port);
    int checkPos = 0;
    while (1) {
        long now = time(NULL);
	for (int i = 0; i < 100; ++i, ++checkPos) {
	    if (checkPos == MAX_EVENTS) checkPos = 0;
	    if (g_Events[checkPos].status != 1) continue;
	    long duration = now - g_Events[checkPos].last_active;
	    if (duration >= 60) {
	        close(g_Events[checkPos].fd);
		printf("[fd = %d] timeout [%d--%d].\n", g_Events[checkPos].fd, g_Events[checkPos].last_active,now);
		EventDel(g_epollFd, &g_Events[checkPos]);
	    
	    }
	
	}
	int fds = epoll_wait(g_epollFd, events, MAX_EVENTS, kTimeOut);
	printf("epoll_wait, fds = %d\n", fds);
	if (fds < 0) {
	    printf("epoll_wait error, exit\n");
	    break;
	}
	for (int i = 0; i < fds; ++i) {
	    myevent_s *ev = (struct myevent_s*) events[i].data.ptr;
	    if ((events[i].events&EPOLLIN)&&(ev->events&EPOLLIN)) {//read event
	        ev->call_back(ev->fd, events[i].events, ev->arg);
	    }
	    if ((events[i].events&EPOLLOUT)&&(ev->events&EPOLLOUT)) {//write event
	        ev->call_back(ev->fd, events[i].events, ev->arg);
	    }
	}
    
    }
    return 0;
}



