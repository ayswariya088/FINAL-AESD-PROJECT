ifeq ($(CC),)
	CC=$(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall -Werror
endif

ifeq ($(LIBS),)
	LIBS= -lgpiod -pthread
endif

all: gpio_led
default: gpio_led

gpio_led :gpio_led.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o gpio_led gpio_led.o $(LIBS)

gpio_led.o : gpio_led.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -c gpio_led.c $(LIBS)

clean :
	rm -f gpio_led *.o
