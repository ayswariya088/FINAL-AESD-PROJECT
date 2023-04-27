all: gpio_led

CROSS_COMPILE = 
CC ?= $(CROSS_COMPILE)gcc

LDFLAGS?= -lpthread -lrt -lgpiod

gpio_led: gpio_led.c
	$(CC) gpio_led.c -Wall -Werror -g -o gpio_led $(LDFLAGS)

clean:
	rm -rf *.o gpio_led
