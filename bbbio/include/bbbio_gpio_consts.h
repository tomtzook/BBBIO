#ifndef BBBIO_GPIOS_H
#define BBBIO_GPIOS_H

#define GPIO_MODULES_COUNT (4)
#define HEADERS_COUNT (2)
#define HEADER_PIN_COUNT (46)

extern const unsigned int gpio_addr_offset[GPIO_MODULES_COUNT];

extern const signed char port_set[HEADERS_COUNT][HEADER_PIN_COUNT];
extern const unsigned char port_id_set[HEADERS_COUNT][HEADER_PIN_COUNT];

#endif //BBBIO_GPIOS_H
