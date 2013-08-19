#include "alarm.h"
#include "menu.h"
#include "time.h"

static time_t the_only_alarm_supported = -1;

void alarm_set_timestamp(alarm_time_set_t * alarm) {
    the_only_alarm_supported = alarm->timestamp;
}

void alarm_run_if_appropriate(time_t timestamp) {
    if (the_only_alarm_supported < timestamp) {
        the_only_alarm_supported = -1;

        struct time_data time_elements;
        split_timestamp(timestamp, &time_elements);
        menu_draw_big_clock(&time_elements);
    }
}
