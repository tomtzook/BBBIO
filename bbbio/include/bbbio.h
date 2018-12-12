#ifndef BBBIO_H_
#define BBBIO_H_

/***********************************************************************\
 * MACROS
\***********************************************************************/

#define BBB_DIR_OUTPUT               (1)
#define BBB_DIR_INPUT                (0)

#define BBB_GPIO_HIGH                (1)
#define BBB_GPIO_LOW                 (0)

#define BBB_HEADER_PIN_COUNT         (46)

#define BBB_P8_HEADER                (0)
#define BBB_P9_HEADER                (1)

#define BBB_PWMSS0                   (0)
#define BBB_PWMSS1                   (1)
#define BBB_PWMSS2                   (2)

/***********************************************************************\
 * FUNCTIONS
\***********************************************************************/

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************\
 * INIT
\***********************************************************************/

int bbbio_init();
void bbbio_free();

/***********************************************************************\
 * GPIO DATA
\***********************************************************************/

char bbbio_gpio_valid(char header, char pin);

signed char bbbio_gpio_module(char header, char pin);
unsigned char bbbio_gpio_pin(char header, char pin);

/***********************************************************************\
 * GPIO IO
\***********************************************************************/

int bbbio_gpio_setdir(char header, char pin, char dir);

void bbbio_gpio_set(char header, char pin, char value);
void bbbio_gpio_high(char header, char pin);
void bbbio_gpio_low(char header, char pin);

char bbbio_gpio_get(char header, char pin);

#ifdef __cplusplus
}
#endif

#endif /* BBBIO_H_ */
