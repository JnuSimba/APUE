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

int setup_daemon(int, int);
/* 守护进程一直在后台运行且无控制终端 */
int main(int argc, char *argv[])
{
//	daemon(0, 0)
	setup_daemon(0, 0);
	printf("test ... \n"); // 无输出
	for(;;) ;
	return 0;
}

int setup_daemon(int nochdir, int noclose)
{
	pid_t pid;
	pid = fork();
	if (pid == -1)
		ERR_EXIT("fork error");
	if (pid > 0)
		exit(EXIT_SUCCESS);
	/* 调用setsid的进程不能为进程组组长，故fork之后将父进程退出 */
	setsid(); // 子进程调用后生成一个新的会话期
	if (nochdir == 0)
		chdir("/"); //更改当前目录为根目录
	if (noclose == 0) {
		int i;
		for (i = 0; i < 3; ++i)
			close(i);
		open("/dev/null", O_RDWR); // 将标准输入，标准输出等都重定向到/dev/null
		dup(0);
		dup(0);
	}

	return 0;
}

