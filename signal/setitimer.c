/*************************************************************************
	> File Name: process_.c
	> Author: Simba
	> Mail: dameng34@163.com 
	> Created Time: Sat 23 Feb 2013 02:34:02 PM CST
 ************************************************************************/
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<signal.h>
#include<sys/time.h>

#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)

void handler(int sig)
{
	printf("recv a sig=%d\n", sig);
}

int main(int argc, char *argv[])
{
	if (signal(SIGALRM, handler) == SIG_ERR)
		ERR_EXIT("signal error");
	
	struct timeval  tv_interval = {1, 0}; //以后每次延时1s
	struct timeval tv_value = {5, 0};//第一次延时5s
	struct itimerval it;
/* Timers decrement from it_value to zero, generate a signal, and reset to it_interval */
	it.it_interval = tv_interval; 
/*	The element it_value is set to the amount of time remaining on the timer */
	it.it_value = tv_value;
	setitimer(ITIMER_REAL, &it, NULL); //间歇性地产生时钟 

	int i;
	for (i = 0; i < 10000; i++) ;
	struct itimerval oit;
	// 上面循环后也许定时时间还没到，重新设置时钟，并将先前时钟剩余值通过oit传出
	setitimer(ITIMER_REAL, &it, &oit);
	/*  getitimer(ITIMER_REAL, &oit); */
	printf("%d %d %d %d\n", (int)oit.it_interval.tv_sec, (int)oit.it_interval.tv_usec,
			(int)oit.it_value.tv_sec, (int)oit.it_value.tv_usec);

	return 0;
}


