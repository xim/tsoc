#ifndef MENU_H
#define MENU_H

#include <stdio.h>

#include "time.h"

inline void menu_init(void);
inline void menu_redraw_clock(struct time_data *);
inline void menu_action(char);
inline void menu_title(const char * title);
inline void menu_content(const char * content);
#endif
