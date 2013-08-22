#ifndef ALARM_H
#define ALARM_H

#include "time.h"
#include "libcwap.h"

void alarm_set_timestamp(alarm_time_set_t *);
void alarm_set_default_timestamp(time_t);
void alarm_run_if_appropriate(time_t);
time_t next_alarm_time(void);
bool alarm_set(void);

#endif
