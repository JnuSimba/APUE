/*************************************************************************
	> File Name: file_cp.c
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

int main(int argc, char *argv[])
{
	int infd;
	int outfd;
	if (argc != 3) {
		fprintf(stderr, "Usage %s src dest\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	infd = open(argv[1], O_RDONLY);
	if (infd == -1)
		ERR_EXIT("open src error");
	if ((outfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1)
		ERR_EXIT("open dest error");

	char buf[1024];
	ssize_t nread;
	while ((nread =read(infd, buf, 1024)) > 0)
		write(outfd, buf, nread); // 可以调用fsync同步内核缓冲区的数据到磁盘文件
								// 或者打开文件时标志为O_SYNC
	close(infd);
	close(outfd);
/********************************************************************************************/
	
	int fd = open("test.txt", O_RDONLY);
	if (fd == -1)
		ERR_EXIT("open error");
	char buf2[1024] = {0};
	int ret = read(fd, buf2, 5);
	if (ret == -1)
		ERR_EXIT("read error");
	ret = lseek(fd, 0, SEEK_CUR); // 从当前位置偏移0个字节
	if (ret == -1)
		ERR_EXIT("lseek");
	printf("current offset=%d\n", ret);

	fd = open("hole.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fd == -1)
		ERR_EXIT("open error");
	write(fd, "ABCDE", 5);
	ret = lseek(fd, 1012*1024*1024, SEEK_CUR);
	if (ret == -1)
		ERR_EXIT("lseek error");
	write(fd, "hello", 5);
	/* 中间的空字符不占用磁盘空间，如ls -lh hole.txt 与 du -h hole.txt
	 * 看到的文件大小不一样*/
	close(fd);

	return 0;
}


