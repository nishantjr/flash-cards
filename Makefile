all: flash-carder

flash-carder: main.c Makefile
	$(CC) $(CFLAGS) $< -std=gnu99 -Wall -g -Werror -o flash-carder
