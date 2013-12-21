.PHONY: clean all
CC=gcc
CFLAGS=-Wall -g
BIN=file_oper file_cp file_ls file_stat file_share file_dup \
	file_fcntl file_flock
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o *~ $(BIN)
