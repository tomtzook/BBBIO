#include <unistd.h>
#include <stdio.h>

// include this to enable debug prints
#include <bbbio_enable_debug.h>

#include <bbbio.h>
#include <bbbio_gpio.h>

int main(){
    // initializing BBBIO context
    bbbio_t bbbio = {0};
    if (bbbio_init(&bbbio) != SUCCESS) {
        printf("error init bbbio \n");
        return 1;
    }

    const bbbio_gpio_header_t header = BBB_P8_HEADER;
    const bbbio_gpio_pin_t pin = 8;

    // setting pin to output
    if (bbbio_gpio_setdir(&bbbio, header, pin, BBB_DIR_OUTPUT) != SUCCESS) {
        printf("failed to set pin to output \n");
        goto free;
    }

    // settings pin output to high
    if (bbbio_gpio_high(&bbbio, header, pin) != SUCCESS) {
        printf("failed to set output to high \n");
        goto free;
    }

    // sleep for 2 seconds
    sleep(2);

    // settings pin output to low
    if (bbbio_gpio_low(&bbbio, header, pin) != SUCCESS) {
        printf("failed to set output to low \n");
        goto free;
    }

free:
    // freeing the BBBIO context, releasing resources
    bbbio_free(&bbbio);

	return 0;
}