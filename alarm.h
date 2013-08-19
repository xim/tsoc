#ifndef ALARM_H
#define ALARM_H

#include "time.h"
#include "libcwap.h"

void alarm_set_timestamp(alarm_time_set_t *);
void alarm_run_if_appropriate(time_t);

#endif
