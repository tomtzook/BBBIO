#ifndef BBBIO_DEBUG_H
#define BBBIO_DEBUG_H

#include <stdio.h>

#define BBBIO_DEBUG(msg) \
    printf(msg)

#define BBBIO_DEBUGF(fmt, args...) \
    printf(fmt, args)

#endif //BBBIO_DEBUG_H
