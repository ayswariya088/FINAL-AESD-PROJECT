all: tmp_sensor

CROSS_COMPILE = 
CC ?= $(CROSS_COMPILE)gcc

LDFLAGS?= -lpthread -lrt

tmp_sensor: tmp_sensor.c
	$(CC) tmp_sensor.c $(LDFLAGS) -Wall -Werror -g -o tmp_sensor

clean:
	rm -rf *.o tmp_sensor
