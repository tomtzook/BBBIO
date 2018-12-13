#include <unistd.h>
#include <stdio.h>

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
    bbbio_gpio_setdir(&bbbio, header, pin, BBB_DIR_OUTPUT);

    // settings pin output to high
    bbbio_gpio_set(&bbbio, header, pin, BBB_GPIO_HIGH);

    // sleep for 2 seconds
    sleep(2);

    // settings pin output to low
    bbbio_gpio_set(&bbbio, header, pin, BBB_GPIO_LOW);

    // freeing the BBBIO context, releasing resources
    bbbio_free(&bbbio);

	return 0;
}