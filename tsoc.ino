#include <stdbool.h>

#include "alarm.h"
#include "keypad.h"
#include "libcwap.h"
#include "menu.h"
#include "time.h"

#define PI_SERIAL Serial
#define PI_SERIALEVENT SerialEvent

struct libcwap_functions cwap_functions = {
    set_time,
    alarm_set_timestamp,
    // etc.
};

inline void wait_for_time_sync(void) {
    keypad_set_action(keypad_set_keypad_pressed);
    menu_content("Waiting for time sync, press any key to skip.");
    while (!time_is_set() && !keypad_pressed())
        delay(50);
}

size_t read_wrapper(char * buf, size_t size) {
    return PI_SERIAL.readBytes(buf, size);
}

void PI_SERIALEVENT() {
    libcwap_action(read_wrapper);
}

void request_time(void) {
    PI_SERIAL.write(CWAP_TIME_REQUEST);
}

void setup(void) {
    menu_init();
    menu_title("Initializing...");
    libcwap_register(&cwap_functions);
    PI_SERIAL.begin(9600);
    request_time();
    set_time_requester(request_time);
    keypad_init();
    wait_for_time_sync();
    keypad_set_action(menu_action);
}

struct time_data current_time_elements;
void loop(void) {
    update_time();
    split_timestamp(get_current_timestamp(), &current_time_elements);
    menu_redraw_clock(&current_time_elements);
    alarm_run_if_appropriate(get_current_timestamp());
    delay(250);
}
