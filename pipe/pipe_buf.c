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

#define TEST_SIZE 68*1024 // 68KB
/* 默认O_NONBLOCK disabled ，这里验证 size > PIPE_BUF(4K)的情况 */
int main(int argc, char *argv[])
{
	char a[TEST_SIZE];
	char b[TEST_SIZE];

	memset(a, 'A', sizeof(a));
	memset(b, 'B', sizeof(b));

	int pipefd[2];
	int ret = pipe(pipefd);
	if (ret == -1)
		ERR_EXIT("pipe error");

	int pid = fork();
	if (pid == 0) {
		
		close(pipefd[0]);
		ret = write(pipefd[1], a, sizeof(a)); // 全部写完才返回
		printf("apid=%d write %d bytes to pipe\n", getpid(), ret);
		exit(0);
	}

	pid = fork();

	if (pid == 0) {
		
		close(pipefd[0]);
		ret = write(pipefd[1], b, sizeof(b));
		printf("bpid=%d write %d bytes to pipe\n", getpid(), ret);
		exit(0);
	}

	close(pipefd[1]);

	sleep(1);

	int fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);
	char buf[1024*4] = {0};
	int n = 1;
	while (1) {
		ret = read(pipefd[0], buf, sizeof(buf)); //当管道被写入数据，就已经可以开始读了,每次读取4k
		if (ret == 0) // 管道写端全部关闭,即读到了结尾
			break;
		printf("n=%02d pid=%d read %d bytes from pipe buf[4095]=%c\n",
				n++, getpid(), ret, buf[4095]);
		write(fd, buf, ret);
	}

	return 0;
}

/*  POSIX.1-2001 says that write(2)s of less than PIPE_BUF bytes must be atomic: the output data is written to
 *  the pipe as a contiguous sequence.  Writes of more than PIPE_BUF bytes may be non-atomic: the  kernel  may
 *  interleave  the  data with data written by other processes.
 */
