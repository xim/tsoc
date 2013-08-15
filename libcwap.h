#ifndef LIBCWAP_H
#define LIBCWAP_H

#include <stddef.h>
#include <inttypes.h>

typedef uint32_t time_t;

#define CWAP_TIME_REQUEST 'T'

struct libcwap_functions {
    void (*time_set_function)(time_t);
    void (*alarm_set_timestamp)(time_t);
    // etc.
};

void libcwap_action(size_t (*)(char *, size_t));
void libcwap_register(struct libcwap_functions *);
#endif
