cmake_minimum_required(VERSION 3.5.1)

project(bbbio C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

set(SOURCES
        src/bbbio.c
        src/bbbio_pwmss.c)

add_library(bbbio
        SHARED
        ${SOURCES})

add_library(bbbio-static
        STATIC
        ${SOURCES})