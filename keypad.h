#ifndef KEYPAD_H
#define KEYPAD_H

#define KEYPAD_COL_PIN_BASE 14
#define KEYPAD_ROW_PIN_BASE 18

#define KEYPAD_NUM_ROWS_COLS 4

#define INTERRUPT_BASE 2

// Most basic functionality – detect if a key has been pressed.
volatile bool keypad_interrupt_queued = false;

// Should be run as soon as possible after keypad_interrupt_queued
void keypad_handle_presses(void);

// Set by keypad_set_keypad_pressed
char keypad_key_pressed = '\0';

// Init the keypad, setting up interrupts/pins
void keypad_init(void);

// Set the action to be run on keypress
void keypad_set_action(void (*)(char));

// Sets the keypad_pressed variable to true on the first button press.
void keypad_set_key_pressed(char);
#endif
