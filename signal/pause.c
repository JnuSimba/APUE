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
	__sighandler_t oldhandler;
	oldhandler = signal(SIGINT, handler); // 返回值是先前的信号处理程序函数指针
	if (oldhandler == SIG_ERR)
		ERR_EXIT("signal error");

	for (; ;) {
		pause(); //使进程挂起直到一个信号被捕获(信号处理函数完成后返回)
				//且调用schedule()使系统调度其他程序运行，
				//这样比完全的死循环的好处是让出cpu
		printf("pause return\n");
	}

	return 0;
}

void handler(int sig)
{
	printf("recv a sig=%d\n", sig);
	sleep(2);
}



