/*************************************************************************
	> File Name: pid_env.c
	> Author: Simba
	> Mail: dameng34@163.com 
	> Created Time: Sun 24 Feb 2013 07:52:09 PM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>

extern char** environ;

int main(void)
{
	printf("hello pid=%d\n", getpid());
	int i;
	for (i = 0; environ[i] != NULL; i++)
		printf("%s\n", environ[i]);
	return 0;
}


