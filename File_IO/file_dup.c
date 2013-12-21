/*************************************************************************
	> File Name: file_dup.c
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

/* dup dup2 fcntl */
int main(int argc, char *argv[])
{
	int fd;
	fd = open("test2.txt", O_WRONLY);
	if (fd == -1)
		ERR_EXIT("open error");
/*
	close(1);
	dup(fd);
*/
//	dup2(fd, 1);
	
	close(1);
	if (fcntl(fd, F_DUPFD, 0) < 0) //从0开始搜索可用的fd
		ERR_EXIT("fcntl error");
	printf("hello\n"); // 输出重定向到test2.txt
	return 0;
}




