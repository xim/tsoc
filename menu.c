#include "time.h"

#include "menu.h"
#include "pcd8544.h"

inline void menu_init(void) {
    pcd8544_clear();
    menu_title("This is the main menu");
}

inline void menu_redraw_clock(struct time_data * time) {
    pcd8544_place_cursor(9, 6);
    char clock[6];
    snprintf(clock, 6, "%d:%d", time->hour, time->minute);
    pcd8544_write_string(clock);
}

inline void menu_action(char button) {
    char data[27];
    snprintf(data, 27, "The button '%c' was pressed", button);
    menu_content(data);
}

inline void menu_title(char * title) {
    pcd8544_place_cursor(0, 0);
    pcd8544_write_string(title);
}

inline void menu_content(char * content) {
    pcd8544_place_cursor(3, 3);
    pcd8544_write_string(content);
}
