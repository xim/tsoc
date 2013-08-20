#ifndef LIBCWAP_H
#define LIBCWAP_H

#include <stdbool.h>
#include <stddef.h>
#include <inttypes.h>

typedef uint32_t timestamp_t;

typedef size_t (*read_function_t)(char *, size_t);

#ifndef PACKED
#define PACKED __attribute__((__packed__))
#endif

#define CWAP_REQUEST_TIME '\t'
#define CWAP_REQUEST_SPEAKING_CLOCK '\v'
#define CWAP_REQUEST_NOISE '\n'
#define CWAP_REQUEST_ALL_ALARMS '\a'

#define CWAP_SET_TIME 'T'
#define CWAP_SET_ACTIONSPEC 'P'
#define CWAP_SET_ALARM_ACTIONS 'A'
#define CWAP_SET_ALARM_TIMESTAMP 'O'
#define CWAP_SET_ALARM_NAME 'N'
#define CWAP_SET_ALARM_REPEAT 'R'
#define CWAP_DELETE_ALARM 'D'

#define CWAP_ALARM_ACTION_COUNT 8
#define CWAP_ACTION_ACTIONSPEC_COUNT 8
#define CWAP_ALARM_NAME_LENGTH 12

typedef union {
    uint32_t mask;
    struct PACKED {
        bool enable_relay_1 : 1;
        bool disable_relay_1 : 1;
        bool enable_relay_2 : 1;
        bool disable_relay_2 : 1;
        bool enable_relay_3 : 1;
        bool disable_relay_3 : 1;
        bool enable_relay_4 : 1;
        bool disable_relay_4 : 1; // eighth bit

        bool blink_backlight : 1;
        bool make_noise : 1;
        bool speaking_clock : 1; // eleventh bit
    } flags;
} actionmask_t;

typedef struct PACKED {
    actionmask_t actions;
    bool snoozability : 1;
    uint16_t timeout : 15; // uint15_t!
    int16_t offset;
} actionspec_t;

typedef struct PACKED {
    uint8_t actionno;
    actionspec_t actionspecs[CWAP_ACTION_ACTIONSPEC_COUNT];
} action_spec_set_t;

typedef struct PACKED {
    uint8_t alarmno;
    uint8_t actionno[CWAP_ALARM_ACTION_COUNT];
} alarm_action_set_t;

typedef struct PACKED {
    uint8_t alarmno;
    char name[CWAP_ALARM_NAME_LENGTH];
} alarm_name_set_t;

typedef union {
    struct PACKED {
        bool monday : 1;
        bool tuesday : 1;
        bool wednesday : 1;
        bool thursday : 1;
        bool friday : 1;
        bool saturday : 1;
        bool sunday : 1;
        bool geek_day : 1; // If we don't name the bit, won't it be sad?
    } days;
    uint8_t mask;
} weekdays_t;

typedef struct PACKED {
    uint8_t alarmno;
    weekdays_t weekdays;
} alarm_repeat_set_t;

typedef struct PACKED {
    uint8_t alarmno;
    timestamp_t timestamp;
} alarm_time_set_t;

struct libcwap_functions {
    void (*time_request_function)(void);
    void (*speaking_clock_request_function)(timestamp_t);
    void (*noise_request_function)(void);
    void (*alarms_request_function)(void);

    void (*time_set_function)(timestamp_t);
    void (*action_spec_set)(action_spec_set_t *);
    void (*alarm_action_set)(alarm_action_set_t *);
    void (*alarm_time_set)(alarm_time_set_t *);
    void (*alarm_name_set)(alarm_name_set_t *);
    void (*alarm_repeat_set)(alarm_repeat_set_t *);
    void (*alarm_delete)(uint8_t);
};

void libcwap_action(read_function_t);
void libcwap_register(struct libcwap_functions *);
#endif
