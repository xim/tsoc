#ifndef ALARM_H
#define ALARM_H

#include "time.h"

void alarm_set_timestamp(uint8_t, time_t);
void alarm_run_if_appropriate(time_t);

#endif
