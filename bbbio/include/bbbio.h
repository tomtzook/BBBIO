#ifndef BBBIO_H_
#define BBBIO_H_

#include "bbbio_rc.h"
#include "bbbio_gpio_consts.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct bbbio_context {
    int mem_fd;

    volatile unsigned int* cm_addr;
    volatile unsigned int* cm_per_addr;
    volatile unsigned int* gpio_addr[BBB_GPIO_MODULES_COUNT];
} bbbio_t;

bbbio_rc_t bbbio_init(bbbio_t* bbbio);
void bbbio_free(bbbio_t* bbbio);

#ifdef __cplusplus
}
#endif

#endif /* BBBIO_H_ */
