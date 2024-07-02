
CC= gcc
# compiler flags:
CFLAGS  = -Wall -ansi -pedantic 

assembler : assembler.c analyze.c assist.c files.c struct.c
	$(CC) $(CFLAGS) -o assembler assembler.c 

clean:
	$(RM) assembler
