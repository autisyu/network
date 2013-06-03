#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<arpa/inet.h>
const int MAX_EVENTS = 10;
const int SERV_PORT  = 1234;
const int LISTENQ    = 5;
const char *LOCAL_ADDR = "127.0.0.1";
int g_epfd,listenFd;
bool setNonBlocking(int &sock)
{
    int opts;
    opts = fcntl(sock, F_GETFL, 0);
    if (opts < 0) {
        perror("fcntl(sock, F_GETFL)");
	return false;
    }

    opts |= O_NONBLOCK;
    if (fcntl(sock, F_SETFL, opts) < 0) {
        perror("fcntl(sock, F_SETFL)");
    } 

    return true;
}
void AcceptCon()
{
    
}
void RecvData()
{
}
void SendData()
{
}
void InitSocktet()
{
    
}
void initsocket( )
{
    struct sockaddr_in clientaddr, servaddr;
    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    //setNonBlocking(listenFd);
    //fcntl(listenFd, F_SETFL, O_NONBLOCK);


    g_epfd = epoll_create(MAX_EVENTS);
    if (g_epfd <= 0) printf("create epoll failed.%d\n", g_epfd);

    //fcntl(g_epfd, F_SETFL, O_NONBLOCK);

    struct epoll_event ev; 
    ev.data.fd = listenFd;
    ev.events  = EPOLLIN | EPOLLET;


    epoll_ctl(g_epfd, EPOLL_CTL_ADD, listenFd, &ev);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    //const char *local_addr = LOCAL_ADDR;
    //inet_aton(local_addr, &(servaddr.sin_addr));
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(SERV_PORT);
    bind(listenFd, (sockaddr*)&servaddr, sizeof(servaddr));
    listen(listenFd, LISTENQ);
}
int main()
{
    struct epoll_event events[MAX_EVENTS];
    initsocket();
    printf("you are the best!\n");
    while(1) {
	//fflush(stdout);
        int nfds = epoll_wait(g_epfd, events, MAX_EVENTS, -1);
	for (int i = 0; i < nfds; ++i) {
	   if ((events[i].events&EPOLLIN) && events[i].data.fd == listenFd) {
	       struct sockaddr_in clientaddrs;
	       socklen_t len  = sizeof(sockaddr_in);
	       int acceptfd = accept(listenFd, (struct sockaddr*)&clientaddrs, &len);
	       epoll_event ev;
	       ev.events = EPOLLIN | EPOLLET;
	       ev.data.fd = acceptfd;
	       fcntl(acceptfd, F_SETFL, O_NONBLOCK);
	       epoll_ctl(g_epfd, EPOLL_CTL_ADD, acceptfd, &ev);
	   //if (events[i].data.fd == listenFd) {
	       printf("accept!\n");
	       //fflush(stdout);
	       /*socklen_t sock_len = sizeof(struct sockaddr_in);
	       int accept_fd = accept(listenFd, (struct sockaddr*)&clientaddr, &sock_len);
	       setNonBlocking(accept_fd);*/
	       //char *str_client_sin_addr = inet_ntoa(clientaddr.sin_addr);
	       /*ev.data.fd = accept_fd;
	       ev.events = EPOLLIN | EPOLLET;
	       epoll_ctl(g_epfd, EPOLL_CTL_ADD, accept_fd, &ev);*/
	       }
	   /*else if (events[i].events&EPOLLIN) {
	       const int max_line = 500;
	       char read_line[max_line];
	       int temp_sock_fd = events[i].data.fd;i*/
	       //int n = read(temp_sock_fd, read_line, max_line);
	       //printf("%s", read_line);
	   }
	//}
    }
    return 0;
}
