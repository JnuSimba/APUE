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

// 判断是否是内部命令，如果是则执行且返回1
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
