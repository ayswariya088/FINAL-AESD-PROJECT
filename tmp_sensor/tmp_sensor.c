#include<stdio.h>
#include<stdlib.h>
#include<linux/i2c-dev.h>
#include<sys/ioctl.h>
#include<fcntl.h>
#include<unistd.h>
/*
 * @function	: main.c for reading data through i2c bus
 *
 * @param		: NULL 
 * @return		: RETURN  0 on success
 *
 */
int main()
{
int file_tmp;
unsigned char tmp_data[2]={0};
char *i2c_bus ="/dev/i2c-1"; // bus Path in RPI we have configured for bus -1
int temp_value;
file_tmp=open(i2c_bus,O_RDWR);
//open the file device
if(file_tmp<0){
printf("Error opening i2c_bus\n");
exit(1);
}

//ioctl call for I2C
ioctl(file_tmp,I2C_SLAVE, 0x48);

//hold line low for starting communication
write(file_tmp,0x00,1);

//read after that from the slave
read(file_tmp,tmp_data,2);

if(read <0){
printf("error while reading data\n");

}
else{
//tmp_sensor value is of 12 bits with  1st byte MSB and 2nd byte only the  d[7] to d[4] are part of the tmp reading
temp_value=((tmp_data[0]<<4) |(tmp_data[1]>>4))*(0.0625); //converting to celcius

printf("temperature value is %d C\n", temp_value);
}
return 0;
}

