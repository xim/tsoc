#include "time.h"

#include "menu.h"
#include "pcd8544.h"

void menu_init(void) {
    pcd8544_clear();
    menu_title("This is the main menu");
}

static inline void menu_redraw_clock(const char * clock) {
    pcd8544_place_cursor(9, 6);
    pcd8544_write_string(clock);
}

void menu_action(char button) {
    last_menu_action_time = current_timestamp;
    char data[27];
    snprintf(data, 27, "The button '%c' was pressed", button);
    menu_content(data);
}

void menu_title(const char * title) {
    last_menu_action_time = current_timestamp;
    pcd8544_place_cursor(0, 0);
    pcd8544_write_string(title);
}

void menu_content(const char * content) {
    last_menu_action_time = current_timestamp;
    pcd8544_place_cursor(3, 3);
    pcd8544_write_string(content);
}

void menu_check_state(void) {
    char clock[6];
    snprintf(clock, 6, "%02lu:%02lu", get_hour(current_timestamp), get_minute(current_timestamp));
    if (last_menu_action_time > (current_timestamp - 30))
        pcd8544_draw_big_clock(clock);
    else
        menu_redraw_clock(clock);
}
