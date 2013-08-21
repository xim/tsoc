#include "relay.h"

void relay_init(void) {
    for (uint8_t i = 0 ; i != RELAY_PIN_COUNT ; i++) {
        pinMode(RELAY_PIN_RANGE_START + i, OUTPUT);
        digitalWrite(RELAY_PIN_RANGE_START + i, 0);
    }
}

void set_relay_state(uint8_t relay, bool state) {
    digitalWrite(RELAY_PIN_RANGE_START + relay, state);
}
