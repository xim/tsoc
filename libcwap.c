#include <stdlib.h>

#include "libcwap.h"

struct libcwap_functions * registered_functions = NULL;

#define HANDLE_VOID(CALLBACK) { \
    if (registered_functions->CALLBACK == NULL) \
        return; \
    registered_functions->CALLBACK(); \
} break;

#define HANDLE_SIMPLE(CALLBACK, DTYPE) { \
    union { \
        char chars[sizeof(DTYPE)]; \
        DTYPE content; \
    } data; \
    if (!read_function(data.chars, sizeof(DTYPE)) || registered_functions->CALLBACK == NULL) \
        return; \
    registered_functions->CALLBACK(data.content); \
} break;

#define HANDLE(CALLBACK, DTYPE) { \
    union { \
        char chars[sizeof(DTYPE)]; \
        DTYPE content; \
    } data; \
    if (!read_function(data.chars, sizeof(DTYPE)) || registered_functions->CALLBACK == NULL) \
        return; \
    registered_functions->CALLBACK(&data.content); \
} break;

void libcwap_action(read_function_t read_function) {
    char action;
    if (!read_function(&action, 1))
        return;

    switch (action) {
        case CWAP_REQUEST_TIME: HANDLE_VOID(time_request_function);
        case CWAP_REQUEST_SPEAKING_CLOCK: HANDLE_SIMPLE(speaking_clock_request_function, time_t);
        case CWAP_REQUEST_NOISE: HANDLE_VOID(noise_request_function);
        case CWAP_REQUEST_ALL_ALARMS: HANDLE_VOID(alarms_request_function);

        case CWAP_SET_TIME: HANDLE_SIMPLE(time_set_function, time_t);
        case CWAP_SET_ACTIONSPEC: HANDLE(action_spec_set, action_spec_set_t);
        case CWAP_SET_ALARM_ACTIONS: HANDLE(alarm_action_set, alarm_action_set_t);
        case CWAP_SET_ALARM_TIMESTAMP: HANDLE(alarm_time_set, alarm_time_set_t);
        case CWAP_SET_ALARM_NAME: HANDLE(alarm_name_set, alarm_name_set_t);
        case CWAP_SET_ALARM_REPEAT: HANDLE(alarm_repeat_set, alarm_repeat_set_t);
        case CWAP_DELETE_ALARM: HANDLE_SIMPLE(alarm_delete, uint8_t);

        default:
            ; // Assume the data was garbage.
    }
}

void libcwap_register(struct libcwap_functions * funs) {
    registered_functions = funs;
}
