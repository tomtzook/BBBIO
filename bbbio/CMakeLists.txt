cmake_minimum_required(VERSION 3.5.1)

project(bbbio C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

set(HEADERS
        include/bbbio.h
        include/bbbio_gpio.h
        include/bbbio_debug.h
        include/bbbio_rc.h
        include/bbbio_gpio_consts.h
        src/bbbio_util.h
        src/bbbio_consts.h
        src/hw.h)

set(SOURCES
        src/bbbio.c
        src/bbbio_gpio.c
        src/bbbio_gpio_consts.c
        )

add_library(bbbio
        SHARED
        ${HEADERS}
        ${SOURCES})

add_library(bbbio-static
        STATIC
        ${HEADERS}
        ${SOURCES})