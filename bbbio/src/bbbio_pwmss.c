/*
 * bbbio_pwmss.c
 *
 *  Created on: Oct 15, 2017
 *      Author: root
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#include <bbbio_pwmss.h>

#include "bbbio_defines.h"
#include "bbbio_global.h"

/***********************************************************************\
 * MODULE DATA
\***********************************************************************/

const unsigned int pwmss_addr_offset[] = {BBBIO_PWMSS0_ADDR, BBBIO_PWMSS1_ADDR, BBBIO_PWMSS2_ADDR};
const unsigned int module_clk_set[] = {BBBIO_CM_EPWMSS0_CLKCTRL, BBBIO_CM_EPWMSS1_CLKCTRL, BBBIO_CM_EPWMSS2_CLKCTRL};

const int clkdiv_vals[] = {1, 2, 4, 8, 16, 32, 64, 128};
const int hspclkdiv_vals[] = {1, 2, 4.0 ,6, 8.0, 10, 12, 14};

/***********************************************************************\
 * GLOBAL VARS
\***********************************************************************/

volatile unsigned int* pwmss_addr[3] = {NULL, NULL, NULL};
volatile unsigned int* epwm_addr[3] = {NULL, NULL, NULL};

/***********************************************************************\
 * INTERNAL
\***********************************************************************/

static int module_clk_check(char module_id){
	return (HWREG(HWADD(cm_addr, BBBIO_PWMSS_CTRL)) & (1 << module_id));
}
static int module_ctrl(char module_id, char enable){
	int success = 1;

	if(enable){
		if(module_clk_check(module_id)){
			HWREG(HWADD(cm_per_addr, module_clk_set[module_id])) = 0x2;
		}else{
			success = 0;

#ifdef BBBIO_DEBUG
			printf("BBBIO PWMSS %d control error: timebase clock disabled in module control \n", module_id);
#endif
		}
	}

	if(!success){
		HWREG(HWADD(cm_per_addr, module_clk_set[module_id])) = (0x3 << 16);
	}

	return success;
}
static int module_calc_TBPRD(float frequency, int* setclkdiv, int* sethspclkdiv){
	int clkdiv = 7;
	int hspclkdiv = 7;
	int tbrd = 0;

	float cycles = 1000000000.0f / frequency;
	float divisor = (cycles / 655350.0f);

	if(divisor <= (128 * 14)){
		int i, j;
		int div;
		for(i = 0; i < 7; ++i){
			for(j = 0; j < 7; ++j){
				div = clkdiv_vals[i] * hspclkdiv_vals[j];
				if(div < (clkdiv_vals[clkdiv] * hspclkdiv_vals[hspclkdiv]) &&
						div > divisor){
					clkdiv = i;
					hspclkdiv = j;
				}
			}
		}

		tbrd = (int)(cycles / (float)(10.0f * (clkdiv_vals[clkdiv] * hspclkdiv_vals[hspclkdiv])));
		*setclkdiv = clkdiv;
		*sethspclkdiv = hspclkdiv;
	}

	return tbrd;
}

/***********************************************************************\
 * INIT
\***********************************************************************/

int bbbio_pwmss_init(){
	if(memh == 0){
#ifdef BBBIO_DEBUG
		printf("BBBIO PWMSS memory not mapped: \n");
#endif
	}

	int err = 0;

	for(int i = 0; i < 3; ++i){
		pwmss_addr[i] = (volatile unsigned int*)mmap(0, BBBIO_PWMSS_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, memh,
				pwmss_addr_offset[i]);
		if(pwmss_addr[i] == MAP_FAILED) {
#ifdef BBBIO_DEBUG
			printf("BBBIO failed mapping PWMSS %d: %s \n", i, strerror(errno));
#endif
			err = 1;
			break;
		}

		epwm_addr[i] = (volatile unsigned int*)HWADD(pwmss_addr[i], BBBIO_PWMSS_EPWM_OFFSET);

		if(!module_ctrl(i, 1)){
#ifdef BBBIO_DEBUG
			printf("BBBIO failed running PWMSS %d clock \n", i);
#endif
			err = 1;
			break;
		}
	}

	if(err){
		//error
		return 1;
	}

#ifdef BBBIO_DEBUG
	printf("BBBIO PWMSS init complete \n");
#endif

	return 0;
}
void bbbio_pwmss_free(){
	if(memh){
		for(int i = 0; i < 3; ++i){
			if(pwmss_addr[i] != NULL){
				munmap((void*)pwmss_addr, BBBIO_PWMSS_LEN);
				pwmss_addr[i] = NULL;
				epwm_addr[i] = NULL;
			}
		}

#ifdef BBBIO_DEBUG
		printf("BBBIO PWMSS shutdown complete \n");
#endif
	}
}

/***********************************************************************\
 * PWMSS control
\***********************************************************************/

void bbbio_pwmss_enable(char module_id){
#ifdef BBBIO_DEBUG
	printf("BBBIO PWMSS %d enable \n", module_id);
#endif

	HWREGH(HWADD(epwm_addr[module_id], BBBIO_EPWM_AQCTLA)) = (0x2 | (0x3 << 4));
	HWREGH(HWADD(epwm_addr[module_id], BBBIO_EPWM_AQCTLB)) = (0x2 | (0x3 << 8));

	HWREGH(HWADD(epwm_addr[module_id], BBBIO_EPWM_TBCNT)) = 0;
	HWREGH(HWADD(epwm_addr[module_id], BBBIO_EPWM_TBCTL)) &= ~0x3;
}
void bbbio_pwmss_disable(char module_id){
#ifdef BBBIO_DEBUG
	printf("BBBIO PWMSS %d disable \n", module_id);
#endif

	HWREGH(HWADD(epwm_addr[module_id], BBBIO_EPWM_TBCTL)) |= 0x3;

	HWREGH(HWADD(epwm_addr[module_id], BBBIO_EPWM_AQCTLA)) = (0x1 | (0x3 << 4));
	HWREGH(HWADD(epwm_addr[module_id], BBBIO_EPWM_AQCTLB)) = (0x1 | (0x3 << 8));

	HWREGH(HWADD(epwm_addr[module_id], BBBIO_EPWM_TBCNT)) = 0;
}

/***********************************************************************\
 * PWMSS IO
\***********************************************************************/

int bbbio_pwmss_settings(char module_id, float frequency, float dutyA, float dutyB){
	if(dutyA < 0.0f || dutyA > 1.0f || dutyB < 0.0f || dutyB > 1.0f){
#ifdef BBBIO_DEBUG
		printf("BBBIO PWMSS %d settings invalid duty cycles \n", module_id);
#endif

		return 1;
	}

	int clkdiv = 0, hspclkdiv = 0;
	int tbrd = module_calc_TBPRD(frequency, &clkdiv, &hspclkdiv);

#ifdef BBBIO_DEBUG
	printf("BBBIO PWMSS %d divisors: TBPRD=%d, CLKDIV=%d, HSPCLKDIV=%d \n", module_id, tbrd, clkdiv, hspclkdiv);
	printf("BBBIO PWMSS %d duty: A=%f, B=%f \n", module_id, dutyA, dutyB);
#endif

	if(tbrd != 0){
#ifdef BBBIO_DEBUG
		printf("BBBIO PWMSS %d settings valid TBPRD \n", module_id);
#endif

		HWREGH(HWADD(epwm_addr[module_id], BBBIO_EPWM_TBCTL)) =
							(BBBIO_TBCTL_CTRMODE_FREEZE | (clkdiv << 10) | (hspclkdiv << 7));

		HWREGH(HWADD(epwm_addr[module_id], BBBIO_EPWM_CMPB)) =
											(unsigned short)((float)tbrd * dutyB);
		HWREGH(HWADD(epwm_addr[module_id], BBBIO_EPWM_CMPA)) =
											(unsigned short)((float)tbrd * dutyA);

		HWREGH(HWADD(epwm_addr[module_id], BBBIO_EPWM_TBPRD)) = (unsigned short)tbrd;

		HWREGH(HWADD(epwm_addr[module_id], BBBIO_EPWM_TBCNT)) = 0;

		return 0;
	}

	return 1;
}
