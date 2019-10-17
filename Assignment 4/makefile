CC=gcc
CFLAGS=-g -std=c99

all:    text2mtf2 mtf2text2

text2mtf2: text2mtf2.o coding2.o
	$(CC) -o text2mtf2 text2mtf2.o coding2.o

mtf2text2: mtf2text2.o coding2.o
	$(CC) -o mtf2text2 mtf2text2.o coding2.o

coding2.o: coding2.c coding2.h
	$(CC) $(CFLAGS) -c coding2.c

text2mtf2.o: text2mtf2.c coding2.h
	$(CC) $(CFLAGS) -c text2mtf2.c

mtf2text2.o: mtf2text2.c coding2.h
	$(CC) $(CFLAGS) -c mtf2text2.c

clean:
	rm -rf text2mtf2.o mtf2text2.o coding2.o text2mtf2 mtf2text2
