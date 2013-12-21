.PHONY: clean all
CC=gcc
CFLAGS=-Wall -g
BIN=process_fork atexit exec pid_env wait system daemon
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)
