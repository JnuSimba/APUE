#ifndef _EXTERNS_H_
#define _EXTERNS_H_
#include "def.h"

extern char cmdline[MAXLINE+1];
extern char avgline[MAXLINE+1];
extern COMMAND cmd[PIPE_LINE];
extern char infile[MAXNAME+1];
extern char outfile[MAXNAME+1];
extern int backgnd;
extern int append;
extern int cmd_count;
extern char* lineptr;
extern char* avptr;
extern pid_t lastpid;

#endif