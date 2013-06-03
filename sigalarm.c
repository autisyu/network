#include<signal.h>
#include<stdio.h>
#include<unistd.h>
static void sig_alrm(int signo)
{
    static int count = 0;
    count++;
    printf("sigalrm times = %d", count);
}
int main()
{
    signal(SIGALRM, sig_alrm); 
    alarm(1);
    while(1){};
    return 0;
}
