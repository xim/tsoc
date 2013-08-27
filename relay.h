#ifndef RELAY_H
#define RELAY_H

#ifndef RELAY_PIN_RANGE_START
#define RELAY_PIN_RANGE_START 22
#endif
#ifndef RELAY_PIN_COUNT
#define RELAY_PIN_COUNT 4
#endif

#include <stdbool.h>
#include <inttypes.h>

void relay_init(void);
void set_relay_state(uint8_t, bool);

#endif
