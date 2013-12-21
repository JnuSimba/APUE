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

#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)
/* 这几个库函数都会调用execve这个系统调用 */
int main(int argc, char *argv[])
{
	char *const args[] = {"ls", "-l", NULL};
	printf("Entering main ... \n");
//	execlp("ls", "ls", "-l", NULL); // 带p会搜索PATH
//	execl("/bin/ls", "ls", "-l", NULL); // 带l为可变参数
//	execvp("ls", args);
	
	int ret = fcntl(1, F_SETFD, FD_CLOEXEC); 
	/* FD_CLOSEXEC被置位为1（在打开文件时标志为O_CLOEXEC也会置位),
	 * 即在执行execve时将标准输出的文件描述符关闭，
	 * 即下面替换的pid_env程序不会在屏幕上输出信息
	 */
	if (ret == -1)
		perror("fcntl error");

	char *const envp[] = {"AA=11", "BB=22", NULL};
	ret = execle("./pid_env", "pid_env", NULL, envp); // 带e可以自带环境变量
	if (ret == -1)
		perror("exec error");
	printf("Exiting main ... \n");

	return 0;
}


