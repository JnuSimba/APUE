#include<stdio.h>
#include<string.h>
#include<signal.h>
#include"init.h"
#include "externs.h"

void int_handler(int sig)
{
	printf("\n[minishell]$ ");
	fflush(stdout);
}

void setup(void)
{
	signal(SIGINT, int_handler);
	signal(SIGQUIT, SIG_IGN);
}

void init(void)
{
	memset(cmdline, 0, sizeof(cmdline));
	memset(cmd, 0, sizeof(cmd));
	memset(avgline, 0, sizeof(avgline));
	lineptr = cmdline;
	avptr = avgline;
	cmd_count = 0;
	backgnd = 0;
	append = 0;
	memset(outfile, 0, sizeof(outfile));
	memset(infile, 0, sizeof(infile));

	int i;
	for (i = 0;i < PIPE_LINE ; ++i )
	{
		cmd[i].infd = 0;
		cmd[i].outfd = 1;
	}

	lastpid = 0;
}