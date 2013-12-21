#include"parse.h"
#include "init.h"
#include "externs.h"
#include "def.h"
#include "execute.h"
#include "builtins.h"
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<linux/limits.h>
#include<fcntl.h>

// shell 主循环
void shell_loop(void)
{
	while(1)
	{
		printf("[minishell]$ ");
		fflush(stdout);
		// 初始化
		init();
		// 读取命令
		if(read_command() == -1) //ctrl+d
			break;
		//解析命令
		if (parse_command() != -1)
		{
			//print_command();
			//执行命令
			execute_command();
		}
		
	}
	printf("\nexit\n");

}

// 读取命令；失败返回-1，成功返回0
int read_command(void)
{
	//按行读取命令,cmdline包含\n字符
	if (fgets(cmdline, MAXLINE, stdin) == NULL)
		return -1;
	
	return 0;
}

// 解析命令；失败返回-1，成功返回命令的个数
int parse_command(void)
{
	// cat < test.txt | grep -n public > test2.txt &
	if (check("\n")) //空命令
		return -1;
	
	//判断是否是内部命令，如果是则执行后马上返回
	if(builtins())
		return -1;

	//1.解析第一条命令
	get_command(0);
	//2.判断是否有输入重定向
	if (check("<"))
		get_name(infile);
	//3.判断是否有管道
	int i;
	for (i = 1; i < PIPE_LINE; i++)
	{
		if (check("|"))
			get_command(i);
		else
			break;
	}
	//4.判断是否有输出重定向
	if(check(">"))
	{
		if(check(">"))
			append = 1;
		get_name(outfile);
	}
	//5.判断是否是后台作业
	if(check("&"))
		backgnd = 1;
	//6.判断是否结束
	if(check("\n"))
	{
		cmd_count = i;
		return cmd_count;
	}
	else
	{
		fprintf(stderr, "command line syntax error.\n");
		return -1;
	}

}

//执行命令；失败返回-1，成功返回0
int execute_command(void)
{
	// 执行外部命令
	execute_disk_command();
	
	return 0;
}

// 将cmdline的命令解析到cmd[i]
//cmd[i]中的args[] 数组中的指针指向avgline中的对应字段
void get_command(int num)
{
	// cat < test.txt | grep -n public > test2.txt &
	int j = 0;
	int inword = 0;
	while(*lineptr != '\0')
	{
		while(*lineptr == ' ' || *lineptr == '\t')
			lineptr++;
		
		/* 将第num个命令的第j个参数指向avptr */
		cmd[num].args[j] = avptr;

		while(*lineptr != ' '
			&& *lineptr != '\t'
			&& *lineptr != '|'
			&& *lineptr != '<'
			&& *lineptr != '>'
			&& *lineptr != '&'
			&& *lineptr != '\n')
		{
				/* 提取参数至avptr指向的avgline数组 */
				*avptr++ = *lineptr++;
				inword = 1;
		}
		*avptr++ = '\0'; //avgline数组中每个命令的参数之间都以'\0'结尾

		switch(*lineptr)
		{
			case ' ':
			case '\t':
				inword = 0;
				j++; //也许此命令还有其他参数
				break;
			case '<':
			case '>':
			case '|':
			case '&':
			case '\n': //完成一个命令的所有参数的提取
				if (inword == 0)
					cmd[num].args[j] = NULL;
				return;
			default:
				return;

		}

	}

}

// 解析得到文件名
void get_name(char* name)
{
	//移过空格
	while(*lineptr == ' ' || *lineptr == '\t')
			lineptr++;

	while(*lineptr != ' '
		&& *lineptr != '\t'
		&& *lineptr != '|'
		&& *lineptr != '<'
		&& *lineptr != '>'
		&& *lineptr != '&'
		&& *lineptr != '\n')
	{
				*name++ = *lineptr++;
	}

	*name = '\0';

}

// 匹配lineptr当前所指字符串是否与str相等
// 若是返回为真且移动lineptr
int check(const char* str)
{
	while(*lineptr == ' ' || *lineptr == '\t')
			lineptr++;

	char* p = lineptr;
	while(*str != '\0' && *p == *str)
	{
		str++;
		p++;
	}

	if(*str == '\0')
	{
		lineptr = p; //移动lineptr
		return 1;
	}

/* lineptr保持不变 */
	return 0;
}

//打印命令参数，调试使用
void print_command(void)
{
	printf("cmd_count=%d\n", cmd_count);
	int i, j;
	for (i = 0; i < cmd_count ; i++ )
	{
		j = 0;
		while (cmd[i].args[j] != NULL)
		{
			printf("[%s] ", cmd[i].args[j]);
			j++;
		}
		printf("\n");
		
	}

	if(infile[0] != '\0')
		printf("infile=[%s]\n", infile);
	if(outfile[0] != '\0')
		printf("outfile=[%s]\n", outfile);
}

