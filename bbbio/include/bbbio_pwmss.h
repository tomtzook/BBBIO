#ifndef BBBIO_PWMSS_H_
#define BBBIO_PWMSS_H_

/***********************************************************************\
 * FUNCTIONS
\***********************************************************************/

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************\
 * INIT
\***********************************************************************/

int bbbio_pwmss_init();
void bbbio_pwmss_free();

/***********************************************************************\
 * PWMSS control
\***********************************************************************/

void bbbio_pwmss_enable(char module_id);
void bbbio_pwmss_disable(char module_id);

/***********************************************************************\
 * PWMSS IO
\***********************************************************************/

int bbbio_pwmss_settings(char module_id, float frequency, float dutyA, float dutyB);

#ifdef __cplusplus
}
#endif

#endif /* BBBIO_PWMSS_H_ */
