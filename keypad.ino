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

char buttons[KEYPAD_NUM_ROWS_COLS][KEYPAD_NUM_ROWS_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

static inline char dequeue_key(void) {
    if (head == tail)
        return '\0';
    char key = presses[head];
    head++;
    head %= FIFO_SIZE;
    return key;
}

static inline void queue_key(char key) {
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

    for (uint8_t i = 0; i != KEYPAD_NUM_ROWS_COLS ; i++)
        digitalWrite(KEYPAD_COL_PIN_BASE + i, HIGH);

    for (uint8_t i = 0; i != KEYPAD_NUM_ROWS_COLS ; i++) {
        digitalWrite(KEYPAD_COL_PIN_BASE + i, LOW);
        for (uint8_t j = 0; j != KEYPAD_NUM_ROWS_COLS ; j++)
            if (!digitalRead(KEYPAD_ROW_PIN_BASE + j))
                // TODO Handle multibutton presses
                queue_key(buttons[i][j]);
        digitalWrite(KEYPAD_COL_PIN_BASE + i, HIGH);
    }

    for (uint8_t i = 0; i != KEYPAD_NUM_ROWS_COLS ; i++)
        digitalWrite(KEYPAD_COL_PIN_BASE + i, LOW);
    interrupts();

    char keypress;
    while (head != tail && (keypress = dequeue_key()) != '\0')
        action_function(keypress);
}

void keypad_init(void) {
    for (uint8_t i = 0; i != KEYPAD_NUM_ROWS_COLS ; i++) {
        pinMode(KEYPAD_COL_PIN_BASE + i, OUTPUT);
        digitalWrite(KEYPAD_COL_PIN_BASE + i, LOW);
        // TODO unsure if INPUT_PULLUP is digital high with high impedance, needs testing.
        pinMode(KEYPAD_ROW_PIN_BASE + i, INPUT_PULLUP);
        // TODO set to CHANGED and handle multiple buttons!
        attachInterrupt(INTERRUPT_BASE + i, handle_interrupt, FALLING);
    }
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
