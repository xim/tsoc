#ifndef MENU_H
#define MENU_H

#include "time.h"

inline void menu_init(void);
inline void menu_redraw_clock(struct time_data *);
inline void menu_action(char);
inline void menu_title(char * title);
inline void menu_content(char * content);
#endif
