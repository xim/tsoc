#include <stdlib.h>

#include "libcwap.h"

struct libcwap_functions * registered_functions = NULL;

inline void libcwap_action(size_t (*read_function)(char *, size_t)) {
    char action;
    if (!read_function(&action, 1))
        return;

    // Remember to increase the buffer if we want to receive larger packets.
    char data[4];
    switch (action) {
        case 'T':
            if (!read_function(data, 4))
                break;
            if (registered_functions->time_set_function != NULL)
                registered_functions->time_set_function(*(time_t *) data); // TODO verify these casts
            break;
        case 'O':
            if (!read_function(data, 4))
                break;
            if (registered_functions->alarm_set_timestamp != NULL)
                registered_functions->alarm_set_timestamp(*(time_t *) data);
            break;
            // etc.
        default:
            ; // Assume the data was garbage.
    }
}

inline void libcwap_register(struct libcwap_functions * funs) {
    registered_functions = funs;
}
