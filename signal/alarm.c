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

#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)

void handler(int sig);

int main(int argc, char *argv[])
{
	if (signal(SIGALRM, handler) == SIG_ERR)
		ERR_EXIT("signal error");
	
	alarm(1); //过了n秒会产生SIGALRM信号
	for (; ;)
		pause();

	return 0;
}

void handler(int sig)
{
	printf("recv a sig=%d\n", sig);
	alarm(1); // 间接递归调用handler
}



