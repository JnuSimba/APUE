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
#include<sys/wait.h>

#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)

int my_system(const char *command);

int main(int argc, char *argv[])
{
	/* 相当于调用 /bin/sh -c ls -l | wc -w */
//	system("ls -l | wc -w");
	my_system("ls -l | wc -w");
	return 0;
}


int my_system(const char* command)
{
	pid_t pid;
	int status;
	if (command == NULL)
		return 1;

	if ((pid = fork()) < 0)
			status = -1;
	else if (pid == 0) {
		execl("/bin/sh", "sh", "-c", command, NULL);
		exit(127);
	}
	else {
		while (waitpid(pid, &status, 0) < 0) {
			if (errno == EINTR)
				continue;
			status = -1;
			break;
		}
	}

	return status;
}



