#include "alarm.h"
#include "keypad.h"
#include "libcwap.h"
#include "menu.h"
#include "time.h"
#include "serial.h"

struct libcwap_functions cwap_functions = {
    .time_request_function = NULL,
    .speaking_clock_request_function = NULL,
    .noise_request_function = NULL,
    .noise_request_stop_function = NULL,
    .alarms_request_function = NULL,

    .time_set_function = set_time,
    .action_spec_set = NULL,
    .alarm_action_set = NULL,
    .alarm_time_set = alarm_set_timestamp,
    .alarm_name_set = NULL,
    .alarm_repeat_set = NULL,
    .alarm_delete = NULL,
};

size_t read_wrapper(char * buf, size_t size) {
    return PI_SERIAL.readBytes(buf, size);
}

static inline void wait_for_time_sync(void) {
    menu_content("\nWaiting for   time sync,    press any key to skip.");
    while (!time_has_been_set && keypad_key_pressed == '\0') {
        if (PI_SERIAL.available())
            libcwap_action(read_wrapper);
        if (keypad_interrupt_queued)
            keypad_handle_presses();
        delay(50);
    }
    menu_clear();
}

static inline void request_time(void) {
    PI_SERIAL.write(CWAP_REQUEST_TIME);
}

void setup(void) {
    menu_init();
    menu_title("Initializing");
    relay_init();
    libcwap_register(&cwap_functions);
    PI_SERIAL.begin(9600);
    set_time_requester(request_time);
    keypad_init();
    wait_for_time_sync();
    keypad_set_action(menu_action);
    menu_clear();
}

void loop(void) {
    update_time();
    alarm_run_if_appropriate();
    menu_check_state();
    if (PI_SERIAL.available())
        libcwap_action(read_wrapper);
    if (keypad_interrupt_queued)
        keypad_handle_presses();
    delay(50);
}
