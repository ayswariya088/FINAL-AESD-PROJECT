/*
 * @File name (gpio_led.c)
 * @File Description: (application to test gpio functioning with LED)
 * @Author Name (AYSWARIYA KANNAN)
 * @Date (04/2372023)
 * @Attributions : referred Project to understand
 *                 how gpiotest and gpio functioning utilized: https://github.com/cu-ecen-aeld/darshitagrawal_hwsingh27_shared/tree/master/gpio
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <gpiod.h>
#include <syslog.h>
#include <string.h>

#define GPIO_PIN (5)

struct gpiod_chip *chip; //struct for getting chip number and line number
struct gpiod_line *line_ptr;
int err;

int main()
{
    chip= gpiod_chip_open("/dev/gpiochip0");

    if (!chip)
    {
        syslog(LOG_ERR,"Error while setting gpio device");
        return -1;
    }

    line_ptr = gpiod_chip_get_line(chip, GPIO_PIN);

    if (!line_ptr) 
    {
        syslog(LOG_ERR,"Error while setting GPIO Line with offset");
        gpiod_chip_close(chip);
        return -1; 
    }

    err = gpiod_line_request_output(line_ptr, "led_testing", 0);

    if (err <0) 
    {
        syslog(LOG_ERR,"Error while setting the output for GPIO");
        gpiod_chip_close(chip);
        return -1;
    }

    err = gpiod_line_set_value(line_ptr, 1);
    if (err <0) 
    {
        syslog(LOG_ERR,"Error while setting the GPIO high");
        gpiod_chip_close(chip);
        return -1;
    }
    printf("GPIO%d is turned on \n", GPIO_PIN);
    syslog(LOG_DEBUG,"GPIO is turned on ");
    sleep(1000000);
    err = gpiod_line_set_value(line_ptr, 0);
    if (err <0) 
    {
        syslog(LOG_ERR,"Error while setting the GPIO low");
        gpiod_chip_close(chip);
        return -1;
    }
    printf("GPIO%d is turned off \n", GPIO_PIN);
    syslog(LOG_DEBUG,"GPIO is turned off ");
    sleep(1000000);

    gpiod_chip_close(chip);

    return 0;
}