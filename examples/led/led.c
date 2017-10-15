#include <unistd.h>
#include <stdio.h>

#include <bbbio.h>

int main(){

	int status;

	//initializing the IO library
	status = bbbio_init();
	if(status){
		printf("INIT FAILED \n");
		return -1;
	}

	//setting pin 8 of header P8 (P8_8) as output
	bbbio_gpio_setdir(BBB_P8_HEADER, 8, BBB_DIR_OUTPUT);

	//setting P8_8 to high digital output, turning on the led
	bbbio_gpio_high(BBB_P8_HEADER, 8);

	//sleep for 2 seconds
	sleep(2);

	//setting P8_8 to low digital output, turning off the led
	bbbio_gpio_low(BBB_P8_HEADER, 8);

	//closing IO library, freeing resources
	bbbio_free();

	return 0;
}