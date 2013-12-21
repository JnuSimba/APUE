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

typedef struct {
	int a;
	int b;
} TEST;

TEST g_data;

void handler(int sig);

int main(int argc, char *argv[])
{
	TEST zeros = {0, 0};
	TEST ones = {1, 1};

	if (signal(SIGALRM, handler) == SIG_ERR)
		ERR_EXIT("signal error");
	
	g_data = zeros;

	alarm(1); //过了n秒会产生SIGALRM信号
	for (; ;) {
		g_data = zeros; // 赋值不是一个原子操作
		g_data = ones;
	}

	return 0;
}

void unsafe_func(void)
{
	/* printf本身就不是Async-Signal-Safe Functions */
	printf("%d %d\n", g_data.a, g_data.b);
}

void handler(int sig)
{
	printf("recv a sig=%d\n", sig);
	unsafe_func(); // 最好不要在信号处理函数中调用不可重入函数
	alarm(1); // 间接递归调用handler
}



