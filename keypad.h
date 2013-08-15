#ifndef KEYPAD_H
#define KEYPAD_H

// Most basic functionality – detect if a key has been pressed.
// Set by keypad_set_keypad_pressed
bool keypad_pressed(void);

// Init the keypad, setting up interrupts/pins
void keypad_init(void);

// Set the action to be run on keypress
void keypad_set_action(void (*)(char));

// Sets the keypad_pressed variable to true on the first button press.
void keypad_set_keypad_pressed(char);
#endif
