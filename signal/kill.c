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
	if (signal(SIGUSR1, handler) == SIG_ERR)
		ERR_EXIT("signal error");
	pid_t pid = fork();
	if (pid == -1)
		ERR_EXIT("fork error");

	if (pid == 0) {
	/*
		pid = getpgrp(); // 得到进程组pid
		kill(-pid, SIGUSR1); //向进程组发送信号
	*/
		killpg(getpgrp(), SIGUSR1);
		exit(EXIT_SUCCESS); // 子进程处理完信号才退出
	}

	int n = 5;
	do {
		n = sleep(5); // sleep会被信号打断，返回unslept的时间
	} while (n > 0);
	
	return 0;
}

void handler(int sig)
{
	printf("recv a sig=%d\n", sig);
}

/* raise(sig) 等价于 kill(getpid(), sig) 给自己发送信号 */

