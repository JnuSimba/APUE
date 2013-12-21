#include "builtins.h"
#include "def.h"
#include "parse.h"
#include<stdio.h>
#include<stdlib.h>

typedef void (*CMD_HANDLER)(void);

typedef struct builtin_cmd
{
	char* name;
	CMD_HANDLER handler;
}BUILTIN_CMD;

BUILTIN_CMD builtins_arr[]=
{
	{"exit", do_exit}, 
	{NULL, NULL}
};

void do_exit(void)
{
	printf("exit\n");
	exit(EXIT_SUCCESS);
}

// �ж��Ƿ����ڲ�����������ִ���ҷ���1
int builtins(void)
{
	int i = 0;
	int found = 0;

	while(builtins_arr[i].name != NULL)
	{
		if(check(builtins_arr[i].name))
		{
			builtins_arr[i].handler();
			found = 1;
			break;
		}
		i++;
	}

	return found;

}
