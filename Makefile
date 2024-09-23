CFLAGS=-O2
CC=cc

SRC = device.c  main.c  translator.c
OBJ = ${SRC:.c=.o}

controller-emu: $(OBJ) config.h
	$(CC) $(CFLAGS) $(OBJ) -o controller-emu

clean:
	rm $(OBJ)
