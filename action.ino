#include "action.h"
#include "menu.h"
#include "relay.h"
#include "pcd8544.h"
#include "serial.h"

static inline void send_cwap_message(char type) {
    PI_SERIAL.write(type);
}

void perform_action(actionmask_t * actions) {
    if (actions->flags.enable_relay_1)
        set_relay_state(0, !actions->flags.inverted);
    if (actions->flags.enable_relay_2)
        set_relay_state(1, !actions->flags.inverted);
    if (actions->flags.enable_relay_3)
        set_relay_state(2, !actions->flags.inverted);
    if (actions->flags.enable_relay_4)
        set_relay_state(3, !actions->flags.inverted);
    if (actions->flags.blink_backlight)
        backlight_blinking = !actions->flags.inverted;

    // TODO can we cancel these? Nope!
    if (actions->flags.make_noise) {
        if (actions->flags.inverted)
            send_cwap_message(CWAP_REQUEST_NOISE_STOP);
        else
            send_cwap_message(CWAP_REQUEST_NOISE);
    }
    if (actions->flags.speaking_clock) {
        if (actions->flags.inverted)
            send_cwap_message(CWAP_REQUEST_NOISE_STOP);
        else
            send_cwap_message(CWAP_REQUEST_SPEAKING_CLOCK);
    }
}
