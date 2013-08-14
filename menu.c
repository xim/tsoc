#include "Time.hpp"

#include "lcd.h"

inline void menu_init(void) {
    lcd_title("This is the main menu");
}

inline void menu_redraw_clock(tmElements_t * time) {
    lcd_write_text(9, 6, "%d:%d", time->Hour, time->Minute);
}

inline void menu_action(char button) {
    lcd_dialog("The button '%c' was pressed", button);
}
