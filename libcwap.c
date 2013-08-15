#include <stdlib.h>

#include "libcwap.h"

struct libcwap_functions * registered_functions = NULL;

typedef union {
    char chars[4];
    uint32_t uinteger;
    int32_t integer;
} data32_t;

void libcwap_action(size_t (*read_function)(char *, size_t)) {
    char action;
    if (!read_function(&action, 1))
        return;

    data32_t data32;
    switch (action) {
        case 'T':
            if (!read_function(data32.chars, 4))
                break;
            if (registered_functions->time_set_function != NULL)
                registered_functions->time_set_function(data32.uinteger); // TODO verify these casts
            break;
        case 'O':
            if (!read_function(data32.chars, 4))
                break;
            if (registered_functions->alarm_set_timestamp != NULL)
                registered_functions->alarm_set_timestamp(data32.uinteger);
            break;
            // etc.
        default:
            ; // Assume the data was garbage.
    }
}

void libcwap_register(struct libcwap_functions * funs) {
    registered_functions = funs;
}
