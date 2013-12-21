/*************************************************************************
	> File Name: sigrtime_recv.c
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
	struct sigaction act;
	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigset_t pset, oldset, newset;
	sigemptyset(&newset);
	sigaddset(&newset, SIGINT);
	sigaddset(&newset, SIGRTMIN);
	sigprocmask(SIG_BLOCK, &newset, &oldset);
	// 将block位置1，因为信号尚未产生，此时未决位都为0
	if (sigaction(SIGINT, &act, NULL) < 0)
		ERR_EXIT("sigaction error");

	if (sigaction(SIGRTMIN, &act, NULL) < 0)
		ERR_EXIT("sigaction error");
	
	if (sigaction(SIGUSR1, &act, NULL) < 0)
		ERR_EXIT("sigaction error");
	
	for (; ;) {
		printf("this is main blockmask\n");
		printsigset(&oldset); //全0
		printsigset(&newset); //两位为1
		printf("this is main sigpending\n");
		sigpending(&pset);
		printsigset(&pset); // 全0
		sleep(1);
	}

	return 0;

}


sigset_t pset, oldset, newset;

void handler(int sig)
{
	if (sig == SIGINT || sig == SIGRTMIN) {

		printf("rev sig=%d\n", sig);
		printf("this is SIGINT | SIGRTMIN blockmask\n");
		printsigset(&oldset); //即main里面的newset
		printsigset(&newset); //block位依然为1
		printf("this is SIGINT | SIGRTMIN sigpending\n");
		sigpending(&pset); // SIGINT or SIGRTMIN的未决位为0
		printsigset(&pset);
	}

	else if (sig == SIGUSR1) {

		sigemptyset(&newset);
		sigaddset(&newset, SIGINT);
		sigaddset(&newset, SIGRTMIN);
		sigprocmask(SIG_UNBLOCK, &newset, &oldset);
		// 上句只是将未决标志位清0,block位依然为1,当处理SIGUSR1期间还会将此位置1
		// 此句结束立马先执行SIGINT or SIGRTMIN 的信号处理函数（开辟新的函数栈)
		// 再接下去执行
		printf("this is SIGUSR1 blockmask\n");
		printsigset(&oldset);
		printsigset(&newset); //block位依然为1
		printf("this is SIGUSR1 sigpending\n");
		sigpending(&pset); //信号全部处理完毕，故未决位都为0
		printsigset(&pset);
	}
}



