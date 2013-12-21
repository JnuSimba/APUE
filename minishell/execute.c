#include "externs.h"
#include "def.h"
#include "execute.h"
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<linux/limits.h>
#include<fcntl.h>

// �������Ǽ�ʵ��ǰ̨��ҵ��shell���̱�����һ�������Ľ�����
// ǰ̨��ҵ���ӽ����Ƕ�����˽������У�ʵ����Ӧ����ǰ̨��ҵ��һ�������Ľ�����
// �����һ����������shell fork�����setpgid��Ϊ�����Ľ������鳤�󣬽��ŵ���tcsetpgrp����Ϊǰ̨�������exec��
// ��������Ӧ���ɵ�һ������fork��exec
// ��ʱ����ǰ̨��ҵ���������ͬһ�������Ľ����飬�������鳤Ϊ��һ������
// �������鳤wait �����ӽ��̣�����˳�
// shell wait ��һ�������˳���tcsetpgrp ���������Լ�Ϊǰ̨�����������������

void execute_disk_command(void)
{
	/* cat < def.h | grep init > aaa.txt */
	int i;
	int fds[2];
	int fd;

	// ����Ǻ�̨��ҵ��������ɽ�ʬ����
	if (backgnd)
		signal(SIGCHLD, SIG_IGN);
	else //��ʱ��ǰ̨��ҵ����Ҫ�ָ��źŴ�������wait���ӽ����˳�
		signal(SIGCHLD, SIG_DFL);

	if (infile[0] != '\0')
		cmd[0].infd = open(infile, O_RDONLY);
	if (outfile[0] != '\0')
	{
		if (append)
			cmd[cmd_count-1].outfd = open(outfile, O_WRONLY | O_CREAT | O_APPEND);
		else
			cmd[cmd_count-1].outfd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC);
	}

	for (i = 0; i < cmd_count ; ++i)
	{
		if (i < cmd_count-1)
		{
			pipe(fds);
			cmd[i].outfd = fds[1];
			cmd[i+1].infd = fds[0];
		}

		forkexec(i);

		//������ÿ�ιرչܵ��Ķ�д�˻��ߴ򿪵��ļ�
		//�ڶ�Ӧ�ӽ����˳���ʱ����ܱ������ͷ�
		if((fd = cmd[i].infd) != 0)
			close(fd);

		if((fd = cmd[i].outfd) != 1)
			close(fd);
	}

// ��Ҫ�ȴ�ǰ̨��ҵ���
	if (backgnd == 0)
	{
	//�ȴ����һ���ӽ����˳�
		while (wait(NULL) != lastpid)
			;
	}
}

// shell fork���ӽ��̺�exec �滻������ִ��
void forkexec(int num)
{
	int i;
	pid_t pid;
	pid = fork();
	if (pid == -1)
		ERR_EXIT("fork");
	if (pid > 0)
	{
		lastpid = pid;
		return;
	}
	else if(pid == 0)
	{
		/* backgnd=1ʱ������һ���������infd�ض�����/dev/null */
		/* ����һ��������ͼ�ӱ�׼�����ȡ���ݵ�ʱ����������EOF */
		/* ����֧����ҵ������bg,fg */
		if (cmd[num].infd == 0 && backgnd == 1)
			cmd[num].infd = open("/dev/null", O_RDONLY);
		
		if(cmd[num].infd != 0)
		{
			close(0);
			dup(cmd[num].infd);
		}
		if (cmd[num].outfd != 1)
		{
			close(1);
			dup(cmd[num].outfd);
		}
		
		for (i = 3; i < FOPEN_MAX; ++i )
			close(i);

		/* �����ǰ̨���̣���Ҫ�ָ��źŵĴ���*/
		if (backgnd == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
		}

		execvp(cmd[num].args[0], cmd[num].args);
		exit(EXIT_FAILURE);
	}
}