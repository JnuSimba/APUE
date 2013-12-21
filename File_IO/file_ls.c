/*************************************************************************
	> File Name: file_ls.c
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
#include<dirent.h>

#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)

int main(int argc, char *argv[])
{
	DIR *dir = opendir(".");
	struct dirent *de;
	while ((de = readdir(dir)) != NULL) {
		if (strncmp(de->d_name, ".", 1) == 0)
				continue; //忽略隐藏文件
		printf("%s\n", de->d_name);
	}
	
	closedir(dir);
	exit(EXIT_SUCCESS); // 等价于return 0
}


