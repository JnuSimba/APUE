/*************************************************************************
	> File Name: file_oper.c
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

// #define ERR_EXIT(m) (perror(m), exit(EXIT_FAILURE))
#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)

/* fileno：将文件指针转换为文件描述符
 * fdopen：将文件描述符转换为文件指针
 */
int main(void)
{
	printf("fileno(stdin) = %d\n", fileno(stdin));

	int fd;
	fd = open("test.txt", O_RDONLY);
/*
	if (fd == -1) {
		// perror("open error"); 
		fprintf(stderr, "open error with errno=%d:%s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
*/
/*
	if (fd == -1)
		ERR_EXIT("open error");
	printf("open success.\n");
*/
	umask(0); //不继承shell的umask值, newmode = mode & ~umask
	fd = open("test.txt", O_WRONLY | O_CREAT, 0666);
	if (fd == -1)
		ERR_EXIT("open error");
	printf("open success.\n");
	close(fd);

	return 0;
}


