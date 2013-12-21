.PHONY: clean all
CC=gcc
CFLAGS=-Wall -g
BIN=signal kill pause alarm reentrant sigprocmask sigaction my_signal \
	sa_mask sigqueue_send sigqueue_recv sigrtime_send sigrtime_recv setitimer
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)
