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

    // setting pin to input
    bbbio_gpio_setdir(&bbbio, header, pin, BBB_DIR_INPUT);

    // reading value from button
    bbbio_gpio_value_t button_value;
    if (bbbio_gpio_get(&bbbio, header, pin, &button_value) != SUCCESS) {
        printf("error getting pin value \n");
        goto free;
    }

    printf("button value: %d \n", button_value);


free:
    // freeing the BBBIO context, releasing resources
    bbbio_free(&bbbio);

    return 0;
}

