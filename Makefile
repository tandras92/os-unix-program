# build an executable named myprog from myprog.c
CC= /usr/bin/gcc
TARGET= shell

all: shell.c
	$(CC) shell.c -o shell
clean:
	rm $(TARGET)

run: shell
	./shell