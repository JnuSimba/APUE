#ifndef _DEF_H_
#define _DEF_H_

#define MAXLINE 1024 //��ȡ����������󳤶�
#define MAXARGS 20 //ÿ��������������
#define PIPE_LINE  5 // һ���ܵ����м�����������
#define MAXNAME 100 //IO�ض����ļ�����󳤶�

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