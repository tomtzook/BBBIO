/*
 * bbbio_defines.h
 *
 *  Created on: Oct 15, 2017
 *      Author: root
 */

#ifndef BBBIO_BBBIO_DEFINES_H_
#define BBBIO_BBBIO_DEFINES_H_

/***********************************************************************\
 * GENERAL
\***********************************************************************/

#define BBBIO_DEBUG

/***********************************************************************\
 * HARDWARE REGISTERS
\***********************************************************************/

#define HWREG(x)                    (*((volatile unsigned int *)(x)))
#define HWADD(x, y)                 (((void*)x) + y)

/***********************************************************************\
 * GPIO
\***********************************************************************/

#define BBBIO_GPIO0_ADDR	        (0x44e07000)
#define BBBIO_GPIO1_ADDR 	        (0x4804c000)
#define BBBIO_GPIO2_ADDR 	        (0x481AC000)
#define BBBIO_GPIO3_ADDR 	        (0x481AE000)
#define BBBIO_GPIOX_LEN 	        (0x1000)

#define BBBIO_GPIO_OE			    (0x134)
#define BBBIO_GPIO_DATAIN 		    (0x138)
#define BBBIO_GPIO_DATAOUT 		    (0x13C)
#define BBBIO_GPIO_CLEARDATAOUT		(0x190)
#define BBBIO_GPIO_SETDATAOUT		(0x194)

#define BBBIO_GPIO_CTRL			    (0x130)
#define BBBIO_GPIO_DEBOUNCENABLE	(0x150)
#define BBBIO_GPIO_DEBOUNCINGTIME	(0x154)

#define BBBIO_CM_PER_ADDR		    (0x44e00000)
#define BBBIO_CM_PER_LEN		    (0x4000)

#define BBBIO_CONTROL_MODULE        (0x44e10000)
#define BBBIO_CONTROL_LEN           (0x2000)


#endif /* BBBIO_BBBIO_DEFINES_H_ */
