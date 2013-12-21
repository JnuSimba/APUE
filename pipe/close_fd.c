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
/* 如果所有管道写端对应的文件描述符被关闭，则read返回0
 * 如果所有管道读端对应的文件描述符被关闭，则write操作会产生信号SIGPIPE
 */
void handler(int sig)
{
	printf("recv sig=%d\n", sig);
}

int main(int argc, char *argv[])
{
	signal(SIGPIPE, handler);

	int pipefd[2];
	if (pipe(pipefd) == -1)
		ERR_EXIT("pipe error");

	pid_t pid;
	pid = fork();
	if (pid == -1)
		ERR_EXIT("fork error");

	if (pid == 0) {
//		close(pipefd[1]);
		close(pipefd[0]);
		exit(EXIT_SUCCESS);
	}

//	close(pipefd[1]);
	close(pipefd[0]);
	sleep(1);
//	char buf[10] = {0};
//	int ret = read(pipefd[0], buf, 10);
	int ret = write(pipefd[1], "hello", 5);
//	printf("ret = %d\n", ret);
	if (ret == -1)
		printf("write error\n");
	
	return 0;
}


