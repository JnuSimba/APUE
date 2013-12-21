/*************************************************************************
	> File Name: sigrtime_send.c
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



int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage %s pid\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	pid_t pid = atoi(argv[1]); //字符串转换为整数
	union sigval val;
	val.sival_int = 100;
	sigqueue(pid, SIGINT, val); // 不可靠信号不会排队，即会丢失
	sigqueue(pid, SIGINT, val);
	sigqueue(pid, SIGINT, val);
	sigqueue(pid, SIGRTMIN, val); //实时信号会排队，即不会丢失
	sigqueue(pid, SIGRTMIN, val);
	sigqueue(pid, SIGRTMIN, val);
	sleep(3);
	kill(pid, SIGUSR1);

	return 0;

}




