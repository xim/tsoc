#include "time.h"

#include "menu.h"
#include "pcd8544.h"

void menu_init(void) {
    pcd8544_clear();
    menu_title("This is the main menu");
}

void menu_redraw_clock(struct time_data * time) {
    pcd8544_place_cursor(9, 6);
    char clock[6];
    snprintf(clock, 6, "%02d:%02d", time->hour, time->minute);
    pcd8544_write_string(clock);
}

void menu_draw_big_clock(struct time_data * time) {
    char clock[6];
    snprintf(clock, 6, "%02d:%02d", time->hour, time->minute);
    pcd8544_draw_big_clock(clock);
}

void menu_action(char button) {
    last_menu_action_time = current_timestamp;
    char data[27];
    snprintf(data, 27, "The button '%c' was pressed", button);
    menu_content(data);
}

void menu_title(const char * title) {
    pcd8544_place_cursor(0, 0);
    pcd8544_write_string(title);
}

void menu_content(const char * content) {
    pcd8544_place_cursor(3, 3);
    pcd8544_write_string(content);
}
