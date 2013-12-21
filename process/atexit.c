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

/* atexit可以注册终止处理程序，ANSI C规定最多可以注册32个终止处理程序。
 * 终止处理程序的调用与注册次序相反 
 * exit()会调用终止处理程序,以及清除IO缓冲区
 */

void my_exit1(void)
{
	printf("my exit1 ... \n");
}

void my_exit2(void)
{
	printf("my exit2 ... \n");
}


int main(int argc, char *argv[])
{
	printf("hello world");
	//fflush(stdout);
	//_exit(0);
	printf("\n");
	atexit(my_exit1);
	atexit(my_exit2);
	exit(0);
}


