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
/* 系统调用signal()实际上调用了sigaction() */
__sighandler_t my_signal(int sig, __sighandler_t handler);

int main(int argc, char *argv[])
{
	my_signal(SIGINT, handler);

	for (; ;)
		pause();

	return 0;

}

__sighandler_t my_signal(int sig, __sighandler_t handler)
{
	struct sigaction act;
	struct sigaction oldact;
	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	if (sigaction(sig, &act, &oldact) < 0)
		return SIG_ERR;

	return oldact.sa_handler; // 返回先前的处理函数指针
}

void handler(int sig)
{
	printf("rev sig=%d\n", sig);
}



