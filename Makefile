CC=gcc
CFLAGS=-g -Wall -Wno-unused-value

all: bus

bus: bus-simulator.c bus.c pintos_headers.h
	$(CC) $(CFLAGS) -o bus bus-simulator.c -lpthread 

test: bus
	./bus
	./bus
	./bus

clean:
	$(RM) bus
	$(RM) -r *.dSYM
