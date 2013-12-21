/*************************************************************************
	> File Name: process_.c
	> Author: Simba
	> Mail: dameng34@163.com 
	> Created Time: Sat 23 Feb 2013 02:34:02 PM CST
 ************************************************************************/
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<signal.h>

#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)

void handler(int sig);
void printsigset(sigset_t* set)
{
	int i;
	for (i = 1; i < NSIG; i++) {
		if (sigismember(set, i))
				putchar('1');
		else
			putchar('0');
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	if (signal(SIGINT, handler) == SIG_ERR)
		ERR_EXIT("signal error");
	if (signal(SIGQUIT, handler) == SIG_ERR)
		ERR_EXIT("signal error");
	
	sigset_t pset; // 64bit
	sigset_t bset;
	sigemptyset(&bset);
	sigaddset(&bset, SIGINT);
	sigprocmask(SIG_BLOCK, &bset, NULL);

	for (; ;) {
		sigpending(&pset);
		printsigset(&pset);
		sleep(1);
	}

	return 0;
}

void handler(int sig)
{
	if (sig == SIGINT)
		printf("recv a sig=%d\n", sig);
	else if (sig == SIGQUIT) {
		printf("rev a sig=%d\n", sig);
		sigset_t uset;
		sigemptyset(&uset);
		sigaddset(&uset, SIGINT);
		sigprocmask(SIG_UNBLOCK, &uset, NULL);
	}

}



