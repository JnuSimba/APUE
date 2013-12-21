/*************************************************************************
	> File Name: file_flock.c
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
	int fd;
	fd = open("test2.txt", O_CREAT | O_RDWR | O_TRUNC, 0664);
	if (fd == -1)
			ERR_EXIT("open error");
	/* 只有对文件有相应的读写权限才能施加对应的文件锁 */
	struct flock lock;
	memset(&lock, 0, sizeof(lock));
	lock.l_type = F_WRLCK; // 排他锁,即不允许其他进程再对其加任何类型的锁，但读锁（共享锁）允许
	lock.l_whence = SEEK_SET;
	lock.l_start = 0; //从文件开头开始锁定
	lock.l_len = 0; // 文件全部内容锁住

	if (fcntl(fd, F_SETLK, &lock) == 0) {
	/* 若为F_SETLKW,这时如果锁已经被其他进程占用，则此进程会阻塞直到其他进程释放锁*/
		printf("lock success\n");
		printf("press any key to unlock\n");
		getchar();
		lock.l_type = F_UNLCK;
		if (fcntl(fd, F_SETLK, &lock) == 0) 
			printf("unlock success\n");
		else
			ERR_EXIT("unlock fail");
	}
	else
		ERR_EXIT("lock fail");

	return 0; //进程退出会对所有文件解锁
}




