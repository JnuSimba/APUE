#include "externs.h"
#include "def.h"
#include "execute.h"
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<linux/limits.h>
#include<fcntl.h>

// 在这里是简单实现前台作业，shell进程本来是一个独立的进程组
// 前台作业的子进程们都归入此进程组中，实际上应该是前台作业是一个独立的进程组
// 比如第一条命令先由shell fork后调用setpgid成为独立的进程组组长后，接着调用tcsetpgrp设置为前台进程组后exec，
// 其他命令应该由第一条命令fork后exec
// 此时所有前台作业的命令都归入同一个独立的进程组，进程组组长为第一条命令
// 进程组组长wait 其他子进程（命令）退出
// shell wait 第一条命令退出后tcsetpgrp 重新设置自己为前台进程组继续接受命令

void execute_disk_command(void)
{
	/* cat < def.h | grep init > aaa.txt */
	int i;
	int fds[2];
	int fd;

	// 如果是后台作业，避免造成僵尸进程
	if (backgnd)
		signal(SIGCHLD, SIG_IGN);
	else //此时是前台作业，需要恢复信号处理，才能wait到子进程退出
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

		//父进程每次关闭管道的读写端或者打开的文件
		//在对应子进程退出的时候才能被真正释放
		if((fd = cmd[i].infd) != 0)
			close(fd);

		if((fd = cmd[i].outfd) != 1)
			close(fd);
	}

// 需要等待前台作业完成
	if (backgnd == 0)
	{
	//等待最后一个子进程退出
		while (wait(NULL) != lastpid)
			;
	}
}

// shell fork出子进程后exec 替换成命令执行
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
		/* backgnd=1时，将第一条简单命令的infd重定向至/dev/null */
		/* 当第一条命令试图从标准输入获取数据的时候立即返回EOF */
		/* 即不支持作业控制如bg,fg */
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

		/* 如果是前台进程，需要恢复信号的处理*/
		if (backgnd == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
		}

		execvp(cmd[num].args[0], cmd[num].args);
		exit(EXIT_FAILURE);
	}
}