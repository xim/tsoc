#include "alarm.h"
#include "menu.h"

static time_t the_only_alarm_supported = -1;

void alarm_set_timestamp(uint8_t alarmno, time_t timestamp) {
    (void) alarmno;
    the_only_alarm_supported = timestamp;
}

void alarm_run_if_appropriate(time_t timestamp) {
    if (the_only_alarm_supported < timestamp) {
        the_only_alarm_supported = -1;
        menu_content("Alarm, alarm!");
    }
}
