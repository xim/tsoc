#include <stdlib.h>

#include "keypad.h"

void (*action_function)(char) = NULL;

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

uint16_t buttons_held = 0;

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
    presses[tail] = key;
    tail = next_tail;
}

static inline void disable_interrupts(void) {
    for (volatile uint8_t i = 0; i != KEYPAD_NUM_ROWS_COLS ; i++)
        detachInterrupt(INTERRUPT_BASE + i);
}

void handle_interrupt(void) {
    keypad_interrupt_queued = true;
    disable_interrupts();
}

static inline void enable_interrupts(void) {
    for (uint8_t i = 0; i != KEYPAD_NUM_ROWS_COLS ; i++)
        attachInterrupt(INTERRUPT_BASE + i, handle_interrupt, CHANGE);
}

#define has_press(row) (!digitalRead(KEYPAD_ROW_PIN_BASE + row))
#define was_pressed(bitmask) (buttons_held & bitmask)
void keypad_handle_presses(void) {
    keypad_interrupt_queued = false;
    if (action_function == NULL) {
        enable_interrupts();
        return;
    }

    for (uint8_t i = 0; i != KEYPAD_NUM_ROWS_COLS ; i++)
        digitalWrite(KEYPAD_COL_PIN_BASE + i, HIGH);

    for (uint8_t i = 0; i != KEYPAD_NUM_ROWS_COLS ; i++) {
        digitalWrite(KEYPAD_COL_PIN_BASE + i, LOW);
        for (uint8_t j = 0; j != KEYPAD_NUM_ROWS_COLS ; j++) {
            uint16_t bitmask = (1 << (i * 4 + j));
            if (has_press(j)) {
                if (!was_pressed(bitmask)) {
                    buttons_held |= bitmask;
                    queue_key(buttons[i][j]);
                }
            } else if (was_pressed(bitmask))
                buttons_held &= (bitmask ^ 0xFFFF);
        }
        digitalWrite(KEYPAD_COL_PIN_BASE + i, HIGH);
    }

    for (uint8_t i = 0; i != KEYPAD_NUM_ROWS_COLS ; i++)
        digitalWrite(KEYPAD_COL_PIN_BASE + i, LOW);
    enable_interrupts();

    char keypress;
    while ((keypress = dequeue_key()) != '\0')
        action_function(keypress);
}

void keypad_set_action(void (*function)(char)) {
    action_function = function;
}

void keypad_set_key_pressed(char key) {
    keypad_key_pressed = key;
}

void keypad_init(void) {
    action_function = keypad_set_key_pressed;
    for (uint8_t i = 0; i != KEYPAD_NUM_ROWS_COLS ; i++) {
        pinMode(KEYPAD_COL_PIN_BASE + i, OUTPUT);
        digitalWrite(KEYPAD_COL_PIN_BASE + i, LOW);
        pinMode(KEYPAD_ROW_PIN_BASE + i, INPUT_PULLUP);
    }
    enable_interrupts();
}
