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

void handler(int, siginfo_t*, void*);


int main(int argc, char *argv[])
{
	struct sigaction act;
	act.sa_sigaction = handler; //sa_sigaction与sa_handler只能取其一
								//sa_sigaction多用于实时信号，可以保存信息
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO; // 设置标志位后可以接收其他进程
							// 发送的数据，保存在siginfo_t结构体中

	if (sigaction(SIGINT, &act, NULL) < 0)
		ERR_EXIT("sigaction error");

	for (; ;)
		pause();

	return 0;

}

void handler(int sig, siginfo_t* info, void* ctx)
{
	printf("recv a sig=%d data=%d data=%d\n", 
			sig, info->si_value.sival_int, info->si_int);
	
}



