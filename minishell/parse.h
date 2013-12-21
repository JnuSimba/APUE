#ifndef PARSE_H_
#define PARSE_H_
#include "def.h"

void shell_loop(void);
int read_command(void);
int parse_command(void);
int execute_command(void);
void get_command(int);
void get_name(char*);
int check(const char*);
void print_command(void);

#endif // parse.h