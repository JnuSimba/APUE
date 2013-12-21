
#include "parse.h"
#include "init.h"
#include "def.h"

char cmdline[MAXLINE+1];
char avgline[MAXLINE+1];
char infile[MAXNAME+1];
char outfile[MAXNAME+1];
char* lineptr;
char* avptr;
int backgnd;
int append;
int cmd_count;
pid_t lastpid;
COMMAND cmd[PIPE_LINE];

int main(void)
{
	//安装信号处理
	setup();
	//进入shell主循环
	shell_loop();

	return 0;
}
