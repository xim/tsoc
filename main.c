#include <stdbool.h>

#include "Time/Time.h"

#include "alarm.h"
#include "keypad.h"
#include "lcd.h"
#include "libcwap.h"
#include "menu.h"

#define SERIAL Serial2
#define SERIALEVENT SerialEvent2

struct libcwap_functions cwap_functions = {
    setTime,
    alarm_set_timestamp,
    // etc.
}

inline void wait_for_time_sync(void) {
    keypad_set_action(keypad_set_keypad_pressed);
    lcd_dialog("Waiting for time sync, press any key to skip.");
    while (timeStatus() == timeNotSet && !keypad_pressed)
        delay_ms(50);
}

SERIALEVENT() {
    libcwap_action(SERIAL.readBytes);
}

inline time_t request_time(void) {
    SERIAL.write(CWAP_TIME_REQUEST);
    return 0;
}

void setup(void) {
    lcd_init();
    lcd_title("Initializing...");
    libcwap_register(cwap_functions);
    SERIAL.begin(9600);
    request_time();
    setSyncProvider(request_time);
    keypad_init();
    wait_for_time_sync();
    menu_init();
    keypad_set_action(menu_action);
}

time_t current_time;
tmElements_t current_time_elements;
void loop(void) {
    breakTime(current_time, &current_time_elements);
    menu_redraw_clock(&current_time_elements);
    alarm_run_if_appropriate(current_time);
    delay_ms(250);
}
