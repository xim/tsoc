#ifndef MENU_H
#define MENU_H

#include <stdio.h>

#include "time.h"

time_t last_menu_action_time = 0;
bool backlight_blinking = false;

void menu_action(char);
void menu_check_state(void);
void menu_content(const char *);
void menu_init(void);
void menu_clear(void);
void menu_title(const char *);
#endif
