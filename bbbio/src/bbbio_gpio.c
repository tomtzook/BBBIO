#include "hw.h"
#include "bbbio_consts.h"

#include <bbbio_gpio_consts.h>
#include <bbbio_debug.h>
#include <bbbio_gpio.h>

#define GPIO_MODULE(header, pin) port_set[header][pin - 1]
#define GPIO_PIN(header, pin) port_id_set[header][pin - 1]

#define CHECK_VALID_GPIO(pin) \
    if (!bbbio_gpio_valid(pin)) { \
        BBBIO_DEBUGF("BBBIO invalid pin %d", pin); \
        return EARG; \
    }


static char bbbio_gpio_valid(pin_t pin){
    return pin > 0 && pin <= 46;
}

bbbio_rc_t bbbio_gpio_setdir(bbbio_t* bbbio, bbbio_gpio_header_t header, pin_t pin, bbbio_gpio_dir_t dir) {
    CHECK_VALID_GPIO(pin);

    signed char module = GPIO_MODULE(header, pin);
    unsigned int module_pin = GPIO_PIN(header, pin);

    if (dir == BBB_DIR_INPUT) {
        HWREG(HWADD(bbbio->gpio_addr[module], BBBIO_GPIO_OE)) |= (1u << module_pin);
    } else if (dir == BBB_DIR_OUTPUT) {
        HWREG(HWADD(bbbio->gpio_addr[module], BBBIO_GPIO_OE)) &= ~(1u << module_pin);
    } else {
        BBBIO_DEBUGF("BBBIO invalid dir %d", dir);
        return EARG;
    }

    return SUCCESS;
}

bbbio_rc_t bbbio_gpio_set(bbbio_t* bbbio, bbbio_gpio_header_t header, pin_t pin, bbbio_gpio_value_t value) {
    switch (value) {
        case BBB_GPIO_HIGH: {
            return bbbio_gpio_high(bbbio, header, pin);
        }
        case BBB_GPIO_LOW: {
            return bbbio_gpio_low(bbbio, header, pin);
        }
    }

    BBBIO_DEBUGF("BBBIO unknown GPIO value %d", value);
    return EARG;
}

bbbio_rc_t bbbio_gpio_high(bbbio_t* bbbio, bbbio_gpio_header_t header, pin_t pin) {
    CHECK_VALID_GPIO(pin);

    signed char module = GPIO_MODULE(header, pin);
    unsigned int module_pin = GPIO_PIN(header, pin);

    HWREG(HWADD(bbbio->gpio_addr[module], BBBIO_GPIO_SETDATAOUT)) = (1u << module_pin);
}

bbbio_rc_t bbbio_gpio_low(bbbio_t* bbbio, bbbio_gpio_header_t header, pin_t pin) {
    CHECK_VALID_GPIO(pin);

    signed char module = GPIO_MODULE(header, pin);
    unsigned int module_pin = GPIO_PIN(header, pin);

    HWREG(HWADD(bbbio->gpio_addr[module], BBBIO_GPIO_CLEARDATAOUT)) = (1u << module_pin);
}

bbbio_rc_t bbbio_gpio_get(bbbio_t* bbbio, bbbio_gpio_header_t header, pin_t pin, bbbio_gpio_value_t *value) {
    CHECK_VALID_GPIO(pin);

    signed char module = GPIO_MODULE(header, pin);
    unsigned int module_pin = GPIO_PIN(header, pin);

    unsigned int raw_value = HWREG(HWADD(bbbio->gpio_addr[module], BBBIO_GPIO_DATAIN)) & (1u << module_pin);
    if (raw_value == 0) {
        *value = BBB_GPIO_LOW;
    } else {
        *value = BBB_GPIO_HIGH;
    }

    return SUCCESS;
}