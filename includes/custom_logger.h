#ifndef CUSTOM_LOGGER_H
#define CUSTOM_LOGGER_H

#include <stdio.h>

#define LOG_MSG(prefix, message, ...) \
    printf("[%s] " message "\n", prefix, ##__VA_ARGS__)

#define SPACING() \
    printf("\n")
#endif