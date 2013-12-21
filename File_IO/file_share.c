/*************************************************************************
	> File Name: file_share.c
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
	int fd1, fd2;
	char buf1[1024] = {0};
	char buf2[1024] = {0};
/* 进程控制块PCB 
 * struct task {
 * ...
 * struct file* filp[1024];
 * }
 * 同一个进程两次打开同一个文件，一个进程拥有的一个文件描述符表其中一个fd索引对应的指针指向一个
 * 文件表(包括文件状态（读写追加同步非阻塞等），当前文件偏移量，
 * 文件引用次数（当有两个fd指向同个文件表时引用计数为2，见dup,也可用于重定向），
 * 文件操作指针， V节点指针等）不共享，
 * V节点表（包括V节点信息（struct stat), i节点信息等）共享 
 */
/* 两个进程打开同一个文件的情况与上类同*/
	fd1 = open("test.txt", O_RDONLY);
	if (fd1 == -1)
		ERR_EXIT("open error");
	read(fd1, buf1, 5);
	printf("buf1=%s\n", buf1);


	fd2 = open("test.txt", O_RDWR);
	if (fd2 == -1)
		ERR_EXIT("open error");
	read(fd2, buf2, 5);
	printf("buf2=%s\n", buf2);
	write(fd2, "AAAAA", 5);

	memset(buf1, 0, sizeof(buf1));
	read(fd1, buf1, 5);
	printf("buf1=%s\n", buf1);
	close(fd1);
	close(fd2);

	return 0;
}


