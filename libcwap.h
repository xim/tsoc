#ifndef LIBCWAP_H
#define LIBCWAP_H

#include <stddef.h>
#include <inttypes.h>

typedef uint32_t time_t;

typedef size_t (*read_function_t)(char *, size_t);

#define CWAP_TIME_REQUEST '\t'
#define CWAP_TIME_SET 'T'
#define CWAP_SET_ALARM_TIMESTAMP 'O'

struct libcwap_functions {
    void (*time_request_function)(void);
    void (*time_set_function)(time_t);
    void (*alarm_set_timestamp)(uint8_t, time_t);
    // etc.
};

void libcwap_action(size_t (*)(char *, size_t));
void libcwap_register(struct libcwap_functions *);
#endif
