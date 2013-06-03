#include<stdio.h>
#include<fcntl.h>
#include<signal.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<netdb.h>
#include<unistd.h>
#include<pthread.h>
#define MAXBUF 1500
int g_keep_alive = 1;
int  g_connect_fd;
int flag = 0;
static void sig_alrm(int signo) {
    printf("sig_alrm\n");
    char *short_message = "$%$";
    int length = send (g_connect_fd,short_message, MAXBUF, 0);
    if (length <= 0) {
        printf("connect to server failed!\n");
	g_keep_alive = 0;
    }
    alarm(1);
}
void* clientRecv(void* arg)
{
    char message[10000];
    int fd = *(int*)arg;
    //while (1) {
        int len = recv(fd, message, MAXBUF, 0);
	if (len > 0) {
	    message[len] = '\0';
	    printf("server message : \n%s", message);
	//}
    }
}
void* clientSnd(void* arg)
{
    
    char *message = "hello world\n";
    /*char message[10000];
    int i;
    for (i = 0; i < 9999; i++) {
        message[i] = 'j';
    }
    message[9999] = '\0';*/
    int fd = *(int*)arg;
    //while (g_keep_alive) {
        //scanf("%s", message);
        //int len = send(fd, message, dwstrlen(message), 0);
        int len = send(fd, message, strlen(message),0);
            len = send(fd, message, strlen(message),MSG_OOB);
	
	if (len > 0) {
	    printf("client send success, len = %d\n", len);
	} else {
	    printf("client send error\n");
	}
	flag = 1;
    //}
}

int main()
{
    char *argv[] = {"192.168.106.202", "1234"};
    int ret;
    char snd_buf[1024];
    int i, port, len;
    static struct sockaddr_in srv_addr;
    port = atoi(argv[1]);
    g_connect_fd= socket(AF_INET, SOCK_STREAM, 0);
    //fcntl(g_connect_fd, F_SETFL, O_NONBLOCK);
    if (g_connect_fd < 0) {
        perror("cannot create communication socket");
	return 1;
    }
    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = inet_addr(argv[0]);
    srv_addr.sin_port = htons(port);
    if ( 0 != connect(g_connect_fd, (struct sockaddr*)&srv_addr, sizeof(srv_addr))) {
        perror("cannot connect to the server");
	close(g_connect_fd);
	return 1;
    }
    /*if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
        return false;
    }*/
    //alarm(1);
    printf("that is why!\n");
    pthread_t recvThread, sndThread;
    void *thread_result;
    pthread_create(&recvThread, NULL, clientRecv, &g_connect_fd);
    //pthread_create(&sndThread, NULL, clientSnd, &g_connect_fd);
    pthread_join(recvThread, &thread_result);
    pthread_join(sndThread, &thread_result);
}
