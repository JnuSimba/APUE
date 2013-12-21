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

// shell ��ѭ��
void shell_loop(void)
{
	while(1)
	{
		printf("[minishell]$ ");
		fflush(stdout);
		// ��ʼ��
		init();
		// ��ȡ����
		if(read_command() == -1) //ctrl+d
			break;
		//��������
		if (parse_command() != -1)
		{
			//print_command();
			//ִ������
			execute_command();
		}
		
	}
	printf("\nexit\n");

}

// ��ȡ���ʧ�ܷ���-1���ɹ�����0
int read_command(void)
{
	//���ж�ȡ����,cmdline����\n�ַ�
	if (fgets(cmdline, MAXLINE, stdin) == NULL)
		return -1;
	
	return 0;
}

// �������ʧ�ܷ���-1���ɹ���������ĸ���
int parse_command(void)
{
	// cat < test.txt | grep -n public > test2.txt &
	if (check("\n")) //������
		return -1;
	
	//�ж��Ƿ����ڲ�����������ִ�к����Ϸ���
	if(builtins())
		return -1;

	//1.������һ������
	get_command(0);
	//2.�ж��Ƿ��������ض���
	if (check("<"))
		get_name(infile);
	//3.�ж��Ƿ��йܵ�
	int i;
	for (i = 1; i < PIPE_LINE; i++)
	{
		if (check("|"))
			get_command(i);
		else
			break;
	}
	//4.�ж��Ƿ�������ض���
	if(check(">"))
	{
		if(check(">"))
			append = 1;
		get_name(outfile);
	}
	//5.�ж��Ƿ��Ǻ�̨��ҵ
	if(check("&"))
		backgnd = 1;
	//6.�ж��Ƿ����
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

//ִ�����ʧ�ܷ���-1���ɹ�����0
int execute_command(void)
{
	// ִ���ⲿ����
	execute_disk_command();
	
	return 0;
}

// ��cmdline�����������cmd[i]
//cmd[i]�е�args[] �����е�ָ��ָ��avgline�еĶ�Ӧ�ֶ�
void get_command(int num)
{
	// cat < test.txt | grep -n public > test2.txt &
	int j = 0;
	int inword = 0;
	while(*lineptr != '\0')
	{
		while(*lineptr == ' ' || *lineptr == '\t')
			lineptr++;
		
		/* ����num������ĵ�j������ָ��avptr */
		cmd[num].args[j] = avptr;

		while(*lineptr != ' '
			&& *lineptr != '\t'
			&& *lineptr != '|'
			&& *lineptr != '<'
			&& *lineptr != '>'
			&& *lineptr != '&'
			&& *lineptr != '\n')
		{
				/* ��ȡ������avptrָ���avgline���� */
				*avptr++ = *lineptr++;
				inword = 1;
		}
		*avptr++ = '\0'; //avgline������ÿ������Ĳ���֮�䶼��'\0'��β

		switch(*lineptr)
		{
			case ' ':
			case '\t':
				inword = 0;
				j++; //Ҳ����������������
				break;
			case '<':
			case '>':
			case '|':
			case '&':
			case '\n': //���һ����������в�������ȡ
				if (inword == 0)
					cmd[num].args[j] = NULL;
				return;
			default:
				return;

		}

	}

}

// �����õ��ļ���
void get_name(char* name)
{
	//�ƹ��ո�
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

// ƥ��lineptr��ǰ��ָ�ַ����Ƿ���str���
// ���Ƿ���Ϊ�����ƶ�lineptr
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
		lineptr = p; //�ƶ�lineptr
		return 1;
	}

/* lineptr���ֲ��� */
	return 0;
}

//��ӡ�������������ʹ��
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

