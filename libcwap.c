#include <stdlib.h>

#include "libcwap.h"

struct libcwap_functions * registered_functions = NULL;

static void handle_time_data_request() {
    if (registered_functions->time_request_function != NULL)
        registered_functions->time_request_function();
}

static void handle_time_set_request(read_function_t read_function) {
    union {
        char chars[4];
        uint32_t timestamp;
    } data;
    if (!read_function(data.chars, 4) || registered_functions->time_set_function == NULL)
        return;
    registered_functions->time_set_function(data.timestamp);
}

static void handle_alarm_set_timestamp_request(read_function_t read_function) {
    union {
        char chars[5];
        struct PACKED {
            uint8_t alarmno;
            uint32_t timestamp;
        } alarm;
    } data;
    // Uncomment when testing alignment on new platforms/compilers.
    //_Static_assert(sizeof data.chars == sizeof data.alarm, "Alignment padding of data detected");
    if (!read_function(data.chars, 5) || registered_functions->alarm_set_timestamp == NULL)
        return;
    registered_functions->alarm_set_timestamp(data.alarm.alarmno, data.alarm.timestamp);
}

void libcwap_action(read_function_t read_function) {
    char action;
    if (!read_function(&action, 1))
        return;

    switch (action) {
        case CWAP_TIME_REQUEST:
            handle_time_data_request();
            break;
        case CWAP_TIME_SET:
            handle_time_set_request(read_function);
            break;
        case CWAP_SET_ALARM_TIMESTAMP:
            handle_alarm_set_timestamp_request(read_function);
            break;
            // etc.
        default:
            ; // Assume the data was garbage.
    }
}

void libcwap_register(struct libcwap_functions * funs) {
    registered_functions = funs;
}
