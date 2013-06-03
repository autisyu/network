/*#include<unistd.h>
#include<stdio.h>
#include<sys/wait.h>
#include<sys/types.h>

void SignHandle(int iSignNo)
{
    printf("Capture sing no:%d\n", iSignNo);
}

int main()
{
    signal(SIGINT, SignHandle);
    while (true) {
        sleep(1);
    }
    return 0;

}*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
//#include <stdlib.h>

int g_iSeq=0;

void SignHandler(int iSignNo)
{
    int iSeq=g_iSeq++;
    printf("%d Enter SignHandler,signo:%d.\n",iSeq,iSignNo);
    sleep(3);
    printf("%d Leave SignHandler,signo:%d\n",iSeq,iSignNo);
}
int main()
{
   struct itimerval value, ovalue;
   value.it_value.tv_sec = 2;
   value.it_value.tv_usec = 0;
   value.it_interval.tv_sec = 3;
   value.it_interval.tv_usec = 0;
   char szBuf[8];
   int iRet;
   alarm(1);
   signal(SIGINT,SignHandler);
   signal(SIGQUIT,SignHandler);
   signal(SIGALRM,SignHandler);
   setitimer(ITIMER_REAL, &value, &ovalue);
   do{
   iRet=read(STDIN_FILENO,szBuf,sizeof(szBuf)-1);
   if(iRet<0){
   perror("read fail.");
   break;
   }
   szBuf[iRet]=0;
   printf("Get: %s",szBuf);
   }while(strcmp(szBuf,"quit\n")!=0);
   return 0;
}
