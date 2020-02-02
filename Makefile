CFLAGS = -O6
CC = gcc
LIBS = -lm

perc:perc.o mtwister.o
	$(CC) $(CFLAGS) -o perc $(LIBS) perc.o mtwister.o

perc.o:perc.c mtwister.h Makefile
	$(CC) $(CFLAGS) -c perc.c

mtwister.o:mtwister.c mtwister.h Makefile
	$(CC) $(CFLAGS) -c mtwister.c