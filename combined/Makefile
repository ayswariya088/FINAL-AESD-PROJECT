ifeq ($(CC),)
	CC=$(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall -Werror
endif

ifeq ($(LIBS),)
	LIBS= -lgpiod -pthread
endif

all: combined_code
default: combined_code

combined_code :combined_code.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o combined_code combined_code.o $(LIBS)

combined_code.o : combined_code.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -c combined_code.c $(LIBS)

clean :
	rm -f combined_code *.o