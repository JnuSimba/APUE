#ifndef _DEF_H_
#define _DEF_H_

#define MAXLINE 1024 //读取的命令行最大长度
#define MAXARGS 20 //每个命令最多个参数
#define PIPE_LINE  5 // 一个管道行中简单命令最多个数
#define MAXNAME 100 //IO重定向文件名最大长度

#include<stdlib.h>
#include<stdio.h>

#define ERR_EXIT(m) \
do\
{ \
	perror(m);\
	exit(EXIT_FAILURE);\
} \
while (0)

typedef struct command
{
	char* args[MAXARGS+1];
	int infd;
	int outfd;
}COMMAND;

#endif