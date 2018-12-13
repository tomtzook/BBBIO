#ifndef BBBIO_GPIOS_H
#define BBBIO_GPIOS_H

#define BBB_GPIO_MODULES_COUNT (4)
#define BBB_HEADERS_COUNT (2)
#define BBB_HEADER_PIN_COUNT (46)

extern const unsigned int bbb_gpio_addr_offset[BBB_GPIO_MODULES_COUNT];

extern const signed char bbb_module_mapping[BBB_HEADERS_COUNT][BBB_HEADER_PIN_COUNT];
extern const unsigned char bbb_pin_mapping[BBB_HEADERS_COUNT][BBB_HEADER_PIN_COUNT];

#endif //BBBIO_GPIOS_H
