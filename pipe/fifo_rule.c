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

int main(int argc, char *argv[])
{
	int fd = open("p2", O_RDONLY);
	if (fd == -1)
		ERR_EXIT("open error");
	printf("open success\n");
	return 0;
}

/* 如果当前打开操作是为读而打开FIFO时
 * O_NONBLOCK disable：阻塞直到有相应进程为写而打开该FIFO
 * O_NONBLOCK enable：立刻返回成功
 * 
 * 如果当前打开操作是为写而打开FIFO时
 * O_NONBLOCK disable：阻塞直到有相应进程为读而打开该FIFO
 * O_NONBLOCK enable：立刻返回失败，错误码为ENXIO
 */
