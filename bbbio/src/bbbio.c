#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <zconf.h>
#include <sys/mman.h>

#include <bbbio_debug.h>
#include <bbbio_rc.h>
#include <bbbio.h>

#include "bbbio_util.h"
#include "bbbio_consts.h"


#define ERROR_MAP_FAILED(ptr, error_goto, return_code_var, return_code, error_msg, error_args...) \
	if (MAP_FAILED == ptr) { \
		BBBIO_DEBUGF(error_msg, error_args); \
		return_code_var = return_code; \
		goto error_goto; \
	}

bbbio_rc_t bbbio_init(bbbio_t* bbbio) {
	if (bbbio->mem_fd) {
		BBBIO_DEBUG("BBBIO already initialized \n");
		return EINITIALIZED;
	}

	bbbio->mem_fd = open("/dev/mem", O_RDWR);
	if (bbbio->mem_fd < 0) {
	    BBBIO_DEBUGF("BBBIO failed to map /dev/mem: %s \n", strerror(errno));
        return EMAP;
	}

	bbbio_rc_t return_code;

	bbbio->cm_per_addr = mmap(0, BBBIO_CM_PER_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, bbbio->mem_fd, BBBIO_CM_PER_ADDR);
	ERROR_MAP_FAILED(bbbio->cm_per_addr, free, return_code, EMAP, "BBBIO failed to map control module per address: %s \n", strerror(errno));

	bbbio->cm_addr = mmap(0, BBBIO_CONTROL_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, bbbio->mem_fd, BBBIO_CONTROL_MODULE);
	ERROR_MAP_FAILED(bbbio->cm_addr, free, return_code, EMAP, "BBBIO failed to map control module: %s \n", strerror(errno));

	for (size_t i = 0; i < GPIO_MODULES_COUNT; ++i) {
		bbbio->gpio_addr[i] = mmap(0, BBBIO_GPIO_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, bbbio->mem_fd, gpio_addr_offset[i]);
        ERROR_MAP_FAILED(bbbio->gpio_addr[i], free, return_code, EMAP, "BBBIO failed to map gpio %zu: %s \n", i, strerror(errno));
	}

	return SUCCESS;
free:
	bbbio_free(bbbio);
	return return_code;
}

void bbbio_free(bbbio_t* bbbio) {
    for (size_t i = 0; i < GPIO_MODULES_COUNT; ++i) {
        MUNMAP(bbbio->gpio_addr[i], BBBIO_GPIO_LEN);
    }

	MUNMAP(bbbio->cm_addr, BBBIO_CONTROL_LEN);
	MUNMAP(bbbio->cm_per_addr, BBBIO_CM_PER_LEN);

    CLOSE_FD(bbbio->mem_fd);
}