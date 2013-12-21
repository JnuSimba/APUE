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
#include<sys/wait.h>

#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)

int main(int argc, char *argv[])
{
	pid_t pid;
	pid = fork();
	if (pid == -1)
		ERR_EXIT("fork error");

	if (pid == 0) {
		sleep(3);
		printf("this is child\n");
//		exit(100);
		abort();	
	}

	printf("this is parent\n");
	int status;
	int ret = wait(&status); // 阻塞等待子进程退出
	/* waitpid可以等待特定的进程，而不仅仅是第一个退出的子进程
	 * 且可以设置option为WNOHANG,即不阻塞等待 */
	printf("ret=%d, pid=%d\n", ret, pid);
	if (WIFEXITED(status))
		printf("child exited normal exit status=%d\n", WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		printf("child exited abnormal signal number=%d\n", WTERMSIG(status));
	else if (WIFSTOPPED(status))
		printf("child stopped signal number=%d\n", WSTOPSIG(status));

	return 0;
}


