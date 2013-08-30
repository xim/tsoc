#ifndef ALARM_H
#define ALARM_H

#include "time.h"
#include "libcwap.h"

void alarm_set_timestamp(alarm_time_set_t *);
void alarm_set_default_timestamp(time_t);
void alarm_run_if_appropriate(void);
time_t next_alarm_time(void);
void alarm_snooze(uint8_t);
void alarm_stop(void);

enum alarm_state {
    alarm_none, alarm_set, alarm_snoozed, alarm_ringing
};

enum alarm_state alarm_state = alarm_none;

#endif
