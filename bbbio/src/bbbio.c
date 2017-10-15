/*
 * bbbio.c
 *
 *  Created on: Oct 14, 2017
 *      Author: root
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include <bbbio.h>

#include "bbbio_defines.h"
#include "bbbio_global.h"
#include "bbbio_pwmss.h"

/***********************************************************************\
 * GPIO PIN DATA
\***********************************************************************/

const signed char port_set[2][46] = {
		{
			-1, -1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 0, 0, 1, 1,
			0, 2, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 0, 0,
			0, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
		},
		{
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0, 1, 1,
			1, 0, 0, 0, 0, 0, 0, 1, 0, 3, 0, 3, 3, 3, 3, 3, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1
		}
};
const unsigned char port_id_set[2][46] = {
		{
			0, 0, 6, 7, 2,	3, 2, 3,
			5, 4, 13, 12, 23, 26, 15,
			14, 27, 1, 22, 31, 30, 5,
			4, 1, 0, 29, 22, 24, 23,
			25, 10, 11, 9, 17, 8, 16,
			14, 15, 12, 13, 10, 11, 8,
			9, 6, 7
		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 30, 28, 31, 18, 16, 19,
			5, 4, 13, 12, 3, 2, 17,
			15, 21, 14, 19, 17, 15, 16,
			14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20,
			7, 0, 0, 0, 0
		}
};

const unsigned int gpio_addr_offset[] = {BBBIO_GPIO0_ADDR, BBBIO_GPIO1_ADDR, BBBIO_GPIO2_ADDR, BBBIO_GPIO3_ADDR};

/***********************************************************************\
 * GLOBAL VARS
\***********************************************************************/

int memh = 0;

volatile unsigned int* cm_addr = NULL;
volatile unsigned int* cm_per_addr = NULL;

volatile unsigned int* gpio_addr[4] = {NULL, NULL, NULL, NULL};

/***********************************************************************\
 * INIT
\***********************************************************************/

int bbbio_init(){
	if (memh) {
#ifdef BBBIO_DEBUG
		printf("BBBIO already initialized (memh != 0)\n");
#endif
		return 1;
	}

	memh = open("/dev/mem", O_RDWR);

	if(memh <= 0){
#ifdef BBBIO_DEBUG
		printf("BBBIO failed to map '/dev/mem' (memh <= 0): %s. Try running with sudo \n", strerror(errno));
#endif
		return 1;
	}

	cm_per_addr = mmap(0, BBBIO_CM_PER_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, memh, BBBIO_CM_PER_ADDR);
	if(cm_per_addr == MAP_FAILED)
    	{
#ifdef BBBIO_DEBUG
		printf("BBBIO failed to map control module per address: %s \n", strerror(errno));
#endif
		return 1;
	}


	int err = 0;
	for (int i = 0; i < 4; ++i) {
		gpio_addr[i] = (volatile unsigned int*)mmap(0, BBBIO_GPIO_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, memh,
													gpio_addr_offset[i]);

		if(gpio_addr[i] == MAP_FAILED) {
#ifdef BBBIO_DEBUG
			printf("BBBIO failed mapping GPIO %d: %s \n", i, strerror(errno));
#endif
			err = 1;
			break;
		}
	}

	if(err){
		return 1;
	}

	cm_addr = mmap(0, BBBIO_CONTROL_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, memh, BBBIO_CONTROL_MODULE);
	if(cm_addr == MAP_FAILED) {
#ifdef BBBIO_DEBUG
		printf("BBBIO failed to map control module: %s \n", strerror(errno));
#endif
		return 1;
	}

	bbbio_pwmss_init();

#ifdef BBBIO_DEBUG
	printf("BBBIO init complete \n");
#endif

	return 0;
}
void bbbio_free(){
	if (memh) {
		bbbio_pwmss_free();

		if(cm_addr != NULL){
			munmap((void*)cm_addr, BBBIO_CONTROL_LEN);
			cm_addr = NULL;
		}
		if(cm_per_addr != NULL){
			munmap((void*)cm_per_addr, BBBIO_CM_PER_LEN);
			cm_per_addr = NULL;
		}

		for (int i = 0; i < 4; ++i) {
			if(gpio_addr[i] != NULL){
				munmap((void*)gpio_addr, BBBIO_GPIO_LEN);
				gpio_addr[i] = NULL;
			}
		}

		close(memh);
		memh = 0;

#ifdef BBBIO_DEBUG
		printf("BBBIO PWMSS shutdown complete \n");
#endif
	}
}

/***********************************************************************\
 * GPIO DATA
\***********************************************************************/

char bbbio_gpio_valid(char header, char pin){
	return bbbio_gpio_module(header, pin) >= 0 && bbbio_gpio_pin(header, pin) > 0;
}

signed char bbbio_gpio_module(char header, char pin){
	if(header < 0 || header > 1 || pin < 1 || pin > 46)
		return -1;
	return port_set[header][pin - 1];
}
unsigned char bbbio_gpio_pin(char header, char pin){
	if(header < 0 || header > 1 || pin < 1 || pin > 46)
		return 0;
	return port_id_set[header][pin - 1];
}

/***********************************************************************\
 * GPIO IO
\***********************************************************************/

int bbbio_gpio_setdir(char header, char pin, char dir){
	if(!bbbio_gpio_valid(header, pin))
		return -1;
	if(dir != BBB_DIR_OUTPUT || dir != BBB_DIR_OUTPUT)
		return -1;

	if (dir == BBB_DIR_OUTPUT) {
#ifdef BBBIO_DEBUG
		printf("BBBIO GPIO output: (%d, %d) \n", header, pin);
#endif

		HWREG(HWADD(gpio_addr[port_set[header][pin - 1]], BBBIO_GPIO_OE)) &= ~(1 << (port_id_set[header][pin - 1]));
	}
	else if (dir == BBB_DIR_INPUT) {
#ifdef BBBIO_DEBUG
		printf("BBBIO GPIO input: (%d, %d) \n", header, pin);
#endif

		HWREG(HWADD(gpio_addr[port_set[header][pin - 1]], BBBIO_GPIO_OE)) |= (1 << (port_id_set[header][pin - 1]));
	}

	return 0;
}

void bbbio_gpio_set(char header, char pin, char value){
	if(value == BBB_GPIO_HIGH){
		bbbio_gpio_high(header, pin);
	}
	else if(value == BBB_GPIO_LOW){
		bbbio_gpio_low(header, pin);
	}
}
void bbbio_gpio_high(char header, char pin){
#ifdef BBBIO_DEBUG
		printf("BBBIO high: (%d, %d) -> (%d, %d) \n", header, pin, port_set[header][pin - 1], port_id_set[header][pin - 1]);
#endif

	HWREG(HWADD(gpio_addr[port_set[header][pin - 1]], BBBIO_GPIO_SETDATAOUT)) = (1 << (port_id_set[header][pin - 1]));
}
void bbbio_gpio_low(char header, char pin){
#ifdef BBBIO_DEBUG
		printf("BBBIO low: (%d, %d) -> (%d, %d) \n", header, pin, port_set[header][pin - 1], port_id_set[header][pin - 1]);
#endif

	HWREG(HWADD(gpio_addr[port_set[header][pin - 1]], BBBIO_GPIO_CLEARDATAOUT)) = (1 << (port_id_set[header][pin - 1]));
}

char bbbio_gpio_get(char header, char pin){
#ifdef BBBIO_DEBUG
		printf("BBBIO get: (%d, %d) -> (%d, %d) \n", header, pin, port_set[header][pin - 1], port_id_set[header][pin - 1]);
#endif

	return (HWREG(HWADD(gpio_addr[port_set[header][pin - 1]], BBBIO_GPIO_DATAIN)) & (1 << (port_id_set[header][pin - 1]))) != 0;
}





