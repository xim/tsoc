#ifndef TIME_H
#define TIME_H

#include <inttypes.h>

typedef uint32_t time_t;

#define LARGEST_TIMESTAMP (0xFFFFFFFF)

// Globally available
static time_t current_timestamp = 0;
static bool time_has_been_set = false;
static time_t measured_time_drift = 0;

enum week_day {
    Monday = 0, Tuesday = 1, Wednesday = 2, Thursday = 3, Friday = 4, Saturday = 5, Sunday = 6
};

struct time_data {
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
  enum week_day week_day;
  //uint8_t day;
  //uint8_t month;
  //uint8_t year;
};

typedef void (*request_time_function_t)(void);

// constants
#define SECS_PER_MIN 60UL
#define MINS_PER_HOUR 60UL
#define SECS_PER_HOUR 3600UL
#define SECS_PER_DAY 86400UL
#define HOURS_PER_DAY 24UL
#define DAYS_PER_WEEK 7UL
#define SECS_PER_WEEK 604800UL

// Conversion macros
#define get_second(time) (time % SECS_PER_MIN)
#define get_minute(time) ((time / MINS_PER_HOUR) % MINS_PER_HOUR)
#define get_hour(time) ((time % SECS_PER_DAY) / SECS_PER_HOUR)
#define get_week_day(time) ((enum week_day)(((time / SECS_PER_DAY + Thursday) % DAYS_PER_WEEK)))
#define minutes_in_seconds(minutes) (minutes * SECS_PER_MIN)
#define hours_in_seconds(hours) (hours * SECS_PER_HOUR)
#define days_in_seconds(days) (days * SECS_PER_DAY)

// Calculation macros
#define elapsed_seconds_today(time) (time % SECS_PER_DAY)
#define last_midnight(time) (time - elapsed_seconds_today(time))
#define next_midnight(time) (last_midnight(time) + SECS_PER_DAY)

time_t get_time(void);
void update_time(void);
void set_time(time_t t);

void set_time_requester(request_time_function_t);
void set_sync_interval(time_t);

void split_timestamp(time_t, struct time_data *);

#endif
