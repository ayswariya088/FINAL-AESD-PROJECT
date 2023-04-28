/*
 * @File name (combined_code.c)
 * @File Description: (Application code for i2c based TMP 102 temperature sensor and gpio led together to control room temperature)
 * @Author Name (AYSWARIYA KANNAN)
 * @Date (04/28/2023)
 * @Attributions : referred Project to understand
 *                 how tmp 102 is interfaced https://github.com/cu-ecen-aeld/final-project-Amey2904dash/wiki/Project-Overview
 *                 how gpiotest and gpio functioning utilized: https://github.com/cu-ecen-aeld/darshitagrawal_hwsingh27_shared/tree/master/gpio
 */

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>
#include <gpiod.h>
/*
 * @function	: main.c for reading data through i2c bus and reflecting upon load using LED
 *
 * @param		: NULL
 * @return		: RETURN  0 on success
 *
 */
#define FILE_PATH "/etc/tmpdir/tempdata.txt"
#define SLAVE_ADDR (0x48)
#define BUFFER_SIZE (50)
#define LOW (0x00)
#define GPIO_PIN (26)

int main()
{

    /**********************GPIO section********/
    struct gpiod_chip *chip; // struct for getting chip number and line number
    struct gpiod_line *line_ptr;
    int err;
    int flag = 0; // flag to set that LED on
    chip = gpiod_chip_open("/dev/gpiochip0");

    if (!chip)
    {
        syslog(LOG_ERR, "Error while setting gpio device");
        return -1;
    }

    line_ptr = gpiod_chip_get_line(chip, GPIO_PIN);

    if (!line_ptr)
    {
        syslog(LOG_ERR, "Error while setting GPIO Line with offset");
        gpiod_chip_close(chip);
        return -1;
    }
    err = gpiod_line_request_output(line_ptr, "led_testing", 0);

    if (err < 0)
    {
        syslog(LOG_ERR, "Error while setting the output for GPIO");
        gpiod_chip_close(chip);
        return -1;
    }

    /**********************************************************************/

    /****************I2C SECTION *******************************************/
    int file_tmp;
    int file_fd; // to open the file for saving temp data
    unsigned char tmp_data[2] = {0};
    char *i2c_bus = "/dev/i2c-1"; // bus Path in RPI we have configured for bus -1
    int temp_value;
    file_tmp = open(i2c_bus, O_RDWR);
    // open the file device
    if (file_tmp < 0)
    {
        syslog(LOG_ERR, "Error opening i2c_bus");
        exit(1);
    }

    // ioctl call for I2C
    if (ioctl(file_tmp, I2C_SLAVE, SLAVE_ADDR) < 0)
    {
        syslog(LOG_ERR, "Error while setting i2c slave device");
        exit(1);
    }
    /*********************************************************************************/
    while (1)
    {

        file_fd = open(FILE_PATH, O_WRONLY | O_APPEND); // in write only and append at the end of file
        if (file_fd < 0)
        {
            syslog(LOG_ERR, "Error opening tempdata.txt file");
            syslog(LOG_ERR, "%s", strerror(errno));
        }
        // hold line low for starting communication
        write(file_tmp, LOW, 1);

        // read after that from the slave
        read(file_tmp, tmp_data, 2);

        if (read < 0)
        {
            syslog(LOG_ERR, "error while reading data");
            printf("error while reading data\n");
            exit(1);
        }
        else
        {
            // tmp_sensor value is of 12 bits with  1st byte MSB and 2nd byte only the  d[7] to d[4] are part of the tmp reading
            temp_value = ((tmp_data[0] << 4) | (tmp_data[1] >> 4)) * (0.0625); // converting to celcius

            if ((temp_value >=25) && (flag == 0))
            {
                err = gpiod_line_set_value(line_ptr, 1);
                if (err < 0)
                {
                    syslog(LOG_ERR, "Error while setting the GPIO high");
                    gpiod_chip_close(chip);
                    return -1;
                }
                flag = 1;
                printf("GPIO%d is turned on \n", GPIO_PIN);
                syslog(LOG_DEBUG, "GPIO is turned on ");
                sleep(1);
            }
            else if ((temp_value < 25) && (flag == 1))
            {
                err = gpiod_line_set_value(line_ptr, 0);
                if (err < 0)
                {
                    syslog(LOG_ERR, "Error while setting the GPIO low");
                    gpiod_chip_close(chip);
                    return -1;
                }
                flag = 0;
                printf("GPIO%d is turned off \n", GPIO_PIN);
                syslog(LOG_DEBUG, "GPIO is turned off ");
                sleep(1);
            }
            // printf("temperature value is %d C\n", temp_value);
            char send_buffer[BUFFER_SIZE];
            memset(&send_buffer[0], 0, sizeof(send_buffer));
            sprintf(send_buffer, "Temperature is %d Deg C\n", temp_value);
            syslog(LOG_DEBUG, "Temperature is %d Deg C ", temp_value);
            write(file_fd, send_buffer, sizeof(send_buffer));

            sleep(1);
        }
        close(file_fd);
        
    }
    gpiod_chip_close(chip);
    return 0;
}
