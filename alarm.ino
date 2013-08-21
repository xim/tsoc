#include "alarm.h"
#include "libcwap.h"
#include "linked_list.h"
#include "menu.h"
#include "time.h"

typedef struct {
    uint8_t actionno;
    linked_list_t * actionspecs;
} action_t;

typedef struct {
    uint8_t alarmno;
    char * name;
    time_t timestamp;
    weekdays_t repetition;
    linked_list_t * actionnos; // uint8_t
} alarm_t;

typedef struct {
    uint8_t alarmno;
    time_t timestamp;
    actionspec_t actionspec;
} action_time_t;

static linked_list_t * actions = NULL;
static linked_list_t * alarms = NULL;
static linked_list_t * action_times = NULL;

static inline alarm_t * new_alarm(uint8_t alarmno, time_t timestamp) {
    alarm_t * empty = MALLOC_TYPE(alarm_t);
    empty->alarmno = alarmno;
    empty->timestamp = timestamp;
    empty->name = NULL;
    empty->repetition.mask = 0;
    empty->actionnos = NULL;
    return empty;
}

static inline action_t * new_action(void) {
    action_t * empty = MALLOC_TYPE(action_t);
    empty->actionno = 0;
    empty->actionspecs = NULL;
    return empty;
}

static inline actionspec_t * new_actionspec(void) {
    actionspec_t * empty = MALLOC_TYPE(actionspec_t);
    empty->timeout = 0;
    empty->offset = 0;
    empty->actions.mask = 0;
    empty->snoozability = 0;
    return empty;
}

static inline alarm_t * get_alarm(uint8_t alarmno) {
    linked_list_t * iterator = alarms;
    while (iterator != NULL) {
        if (GET_MEMBER(iterator, alarm_t, alarmno) == alarmno)
            break;
        NEXT(iterator);
    }
    return GET_ITEM(iterator, alarm_t);
}

static inline void prepare_action(alarm_t * alarm, actionspec_t * actionspec) {
    time_t action_timestamp = alarm->timestamp + actionspec->offset;

    linked_list_t * insert_pos = NULL;
    linked_list_t * iterator = action_times;
    while (iterator != NULL && GET_MEMBER(iterator, action_time_t, timestamp) < action_timestamp) {
        insert_pos = iterator;
        NEXT(iterator);
    }
    action_time_t * action_time = MALLOC_TYPE(action_time_t);
    action_time->alarmno = alarm->alarmno;
    action_time->timestamp = action_timestamp;
    action_time->actionspec = *actionspec;

    if (insert_pos == NULL)
        push_front(action_times, action_time);
    else
        push_behind(insert_pos, action_time);
}

static inline void clear_actions(uint8_t alarmno) {
    linked_list_t * prev_iter = NULL;
    linked_list_t * iterator = action_times;
    while (iterator != NULL) {
        if (GET_MEMBER(action_times, action_time_t, alarmno) == alarmno) {
            if (prev_iter == NULL)
                action_times = iterator->next;
            else
                prev_iter->next = iterator->next;
            free(iterator);
        }
        prev_iter = iterator;
        NEXT(iterator);
    }
}

void alarm_set_timestamp(alarm_time_set_t * alarm_req) {
    clear_actions(alarm_req->alarmno);

    alarm_t * alarm = get_alarm(alarm_req->alarmno);

    if (alarm == NULL) {
        alarm = new_alarm(alarm_req->alarmno, alarm_req->timestamp);
        alarm->name = strdup("Default");
        push_front(alarms, (void *) alarm);
        action_t * action;
        if (actions != NULL)
            action = GET_ITEM(actions, action_t);
        else {
            actionspec_t * actionspec = new_actionspec();
            actionspec->actions.flags.blink_backlight = true;
            actionspec->snoozability = true;
            actionspec->timeout = 0x7FFF;

            action = new_action();
            push_front(action->actionspecs, (void *) actionspec);
            push_front(actions, (void *) action);
            push_front(alarm->actionnos, COPY_ITEM(uint8_t, &action->actionno));
        }
        push_front(alarm->actionnos, COPY_ITEM(uint8_t, &action->actionno));
    }

    prepare_action(alarm, new_actionspec());
    linked_list_t * iterator = alarm->actionnos;
    while (iterator != NULL) {
        uint8_t actionno = *GET_ITEM(iterator, uint8_t);
        linked_list_t * action_iterator = actions;
        while (action_iterator != NULL) {
            if (actionno == GET_MEMBER(action_iterator, action_t, actionno)) {
                linked_list_t * actionspec_iterator = GET_MEMBER(action_iterator, action_t, actionspecs);
                while (actionspec_iterator != NULL) {
                    prepare_action(alarm, GET_ITEM(actionspec_iterator, actionspec_t));
                    NEXT(actionspec_iterator);
                }
            }
            NEXT(action_iterator);
        }
        NEXT(iterator);
    }
}

static inline time_t next_repeat(time_t time, weekdays_t repetition) {
    uint8_t day = get_week_day(time);
    for (uint8_t i = day + 1; i != day; i = ((i + 1) % DAYS_PER_WEEK))
        if (repetition.mask & (1 << i))
            return time + SECS_PER_DAY * ((i - day) % 7);
    return day + SECS_PER_DAY;
    // TODO Here, we should find the next geek day! For now, treat geek_day as every_day!
}

static inline void perform_action(actionspec_t * actionspec) {
    // TODO Actually do what the spec says.
    (void) actionspec;
    struct time_data time_elements;
    split_timestamp(0, &time_elements);
    menu_draw_big_clock(&time_elements);
}

void alarm_run_if_appropriate(time_t timestamp) {
    if (action_times == NULL)
        return;
    action_time_t * action_time = GET_ITEM(action_times, action_time_t);
    if (action_time->timestamp > timestamp)
        return;

    // TODO document that empty actions == actual alarm time?
    if (!action_time->actionspec.actions.mask) {
        alarm_t * alarm = get_alarm(action_time->alarmno);
        if (alarm != NULL && alarm->repetition.mask) { // Not having an alarm here should be impossible?
            alarm_time_set_t next_run;
            next_run.alarmno = alarm->alarmno;
            next_run.timestamp = next_repeat(alarm->timestamp, alarm->repetition);
            alarm_set_timestamp(&next_run);
        }
        // TODO Remember to keep meta of where we are – are any actions
        // running, are we snoozed, etc.
        menu_title("Alarm ringing:");
        if (alarm != NULL && alarm->name != NULL)
            menu_content(alarm->name);
        else
            menu_content("Unnamed");
    } else
        perform_action(&action_time->actionspec);

    pop_head(action_times);

}
