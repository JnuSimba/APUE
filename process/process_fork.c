/*************************************************************************
	> File Name: process_fork.c
	> Author: Simba
	> Mail: dameng34@163.com 
	> Created Time: Sat 23 Feb 2013 02:34:02 PM CST
 ************************************************************************/
/* 如果父进程先退出，子进程还没退出那么子进程的父进程将变为init进程。（注：任何一个进程都必须有父进程）
 * 如果子进程先退出，父进程还没退出，那么子进程必须等到父进程捕获到了子进程的退出状态才真正结束，
 * 否则这个时候子进程就成为僵进程。
 */
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
	signal(SIGCHLD, SIG_IGN); // 避免产生僵尸进程，忽略SIGCHLD信号
	printf("before fork pid=%d\n", getpid());
	int fd;
	fd = open("test.txt", O_WRONLY);
	if (fd == -1)
		ERR_EXIT("open error");

	pid_t pid;
	pid = fork(); // 写时复制copy on write，只读代码段可以共享
	/* 若使用vfork()则再还没调用exec之前，父子进程是共享同一个地址空间，
	 * 不像fork()一样会进行拷贝 */
	if (pid == -1)
		ERR_EXIT("fork error");

	if (pid > 0) {
		printf("this is parent\n");
		printf("parent pid=%d child pid=%d\n", getpid(), pid);
		write(fd, "parent", 6); // 父子进程共享一个文件表
		sleep(10);
	}

	else if (pid == 0) {
		printf("this is child\n");
		printf("child pid=%d parent pid=%d\n",getpid(), getppid());
		write(fd, "child", 5);
	}

	return 0;
}


