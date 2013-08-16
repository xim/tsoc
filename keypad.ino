#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "keypad.h"

void (*action_function)(char) = keypad_set_keypad_pressed;

bool keypad_has_been_pressed = false;

#define FIFO_SIZE 8
static char presses[FIFO_SIZE];
static uint8_t head = 0;
static uint8_t tail = 0;

static inline char pop_key(void) {
    if (head == tail)
        return '\0';
    char key = presses[head];
    head++;
    head %= FIFO_SIZE;
    return key;
}

static inline void push_key(char key) {
    uint8_t next_tail = (tail + 1) % FIFO_SIZE;
    if (next_tail == head)
        return;
    tail = next_tail;
    presses[tail] = key;
}

void handle_interrupt(void) {
    noInterrupts();
    if (action_function == NULL) {
        interrupts();
        return;
    }

    // TODO actually read keys

    while (head != tail)
        action_function(pop_key());
    interrupts();
}

void keypad_init(void) {
    // Set up the rows and colums as outputs and inputs
    // Set up interrupts
    attachInterrupt(0, handle_interrupt, CHANGE); // LOW, CHANGE, RISING, FALLING
    attachInterrupt(1, handle_interrupt, CHANGE);
    attachInterrupt(2, handle_interrupt, CHANGE);
    attachInterrupt(3, handle_interrupt, CHANGE);
}

void keypad_set_action(void (*function)(char)) {
    action_function = function;
}

void keypad_set_keypad_pressed(char key) {
    (void) key;
    keypad_has_been_pressed = true;
}

bool keypad_pressed(void) {
    return keypad_has_been_pressed;
}
