#include <avr/io.h>
#include <avr/interrupt.h>

void (*action_function)(char) = NULL;

static char presses[8];
static uint8_t head = 0;
static uint8_t tail = 0;

static inline char pop_key(void) {
    if (head == tail)
        return '\0';
    char key = presses[head];
    head++;
    head %= 8;
    return key;
}

static inline void push_key(char key) {
    if ((tail + 1) % 8 == head)
        return;
    tail++;
    tail %= 8;
    presses[tail] = key;
}

ISR(INT0_vect) {
    // do interrupt magic =)
}

static inline void handle_press(void) {
    if (action_function != NULL)
        action_function(pop_key());
    else
        pop_key();
}

inline void keypad_init(void) {
    // Set up the rows and colums as outputs and inputs
    // Set up interrupts
}

inline void keypad_set_action(void (*function)(char)) {
    action_function = function;
}

inline void keypad_set_keypad_pressed(char) {
    keypad_pressed = true;
}
