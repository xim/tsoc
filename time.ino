#include "time.h"

static uint32_t sync_interval = 300;
static uint32_t previous_millis = 0;
static uint32_t next_sync_threshold = 0;

static request_time_function_t time_requester = NULL;

void split_timestamp(time_t time, struct time_data * data){
    data->second = time % SECS_PER_MIN;
    time /= SECS_PER_MIN;
    data->minute = time % MINS_PER_HOUR;
    time /= MINS_PER_HOUR;
    data->hour = time % HOURS_PER_DAY;
    time /= HOURS_PER_DAY;
    data->week_day = ((time + Thursday) % DAYS_PER_WEEK);
}

void update_time(void) {
    time_t current_millis = millis();
    while (current_millis > previous_millis)
        current_timestamp++, previous_millis += 1000;

    if (next_sync_threshold <= current_timestamp && time_requester != NULL)
        (*time_requester)(), next_sync_threshold = current_timestamp + sync_interval;
}

time_t get_time(void) {
    return current_timestamp;
}

void set_time(time_t t) {
    update_time();
    next_sync_threshold = t + sync_interval;
    if (time_has_been_set) {
        measured_time_drift += current_timestamp - t;
    } else {
        time_has_been_set = true;
        previous_millis = millis();
    }
    current_timestamp = t;
}

void set_time_requester(request_time_function_t requester) {
    time_requester = requester;
    (*requester)();
}

void set_sync_interval(time_t interval) {
    next_sync_threshold += interval - sync_interval;
    sync_interval = interval;
}
