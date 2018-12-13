#include <unistd.h>
#include <stdio.h>

#include <bbbio.h>
#include <bbbio_gpio.h>

int main(){
    bbbio_t bbbio = {0};
    if (bbbio_init(&bbbio) != SUCCESS) {
        printf("error init bbbio \n");
        return 1;
    }

    bbbio_gpio_setdir(&bbbio, BBB_P8_HEADER, 1, BBB_DIR_OUTPUT);

    bbbio_gpio_set(&bbbio, BBB_P8_HEADER, 2, BBB_GPIO_HIGH);

    bbbio_free(&bbbio);

	return 0;
}