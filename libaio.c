#include<libaio.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
int main(void) 
{
    int output_fd;
    const char *content      = "hello world!";
    const char *outputfile   = "hello.txt";
    io_context_t ctx;
    struct iocb io;
    struct iocb *p   = &io;
    struct io_event e;
    struct timespec timeout;
    memset(&ctx, 0, sizeof(ctx));
    if (io_setup(10, &ctx) != 0) {
        printf("io_setup error\n");
	return -1;
    }
    if ((output_fd = open(outputfile, O_CREAT | O_WRONLY, 0644)) < 0) {
        perror("open error");
	io_destroy(ctx);
	return -1;
    }

    io_prep_pwrite(&io, output_fd, content, strlen(content), 0);
    io.data = content;
    if (io_submit(ctx, 1, &p) != 1) {
        io_destroy(ctx);
	perror("io_submit error");
	return -1;
    }
    while (1) {
        timeout.tv_sec  = 0;
	timeout.tv_nsec = 500000000;
	if (io_getevents(ctx, 0, 1, &e, &timeout) == 1) {
	   close(output_fd);
	   break;
	}
	printf("haven't done\n");
	sleep(1);
    } 
    io_destroy(ctx);
    return 0;
}
