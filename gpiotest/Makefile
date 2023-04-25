all: sample

CROSS_COMPILE = 
CC ?= $(CROSS_COMPILE)gcc

LDFLAGS?= -lpthread -lrt

sample: sample.c
	$(CC) sample.c $(LDFLAGS) -Wall -Werror -g -o sample

clean:
	rm -rf *.o sample
