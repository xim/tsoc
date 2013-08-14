#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include "pcd8544/pcd8544.h"

// TODO use Arduino GPIO, not wiringpi:
// digitalWrite
// pinMode
// constants

// TODO use a PCD8544 lib actually made for Arduino

struct coordinate {
    uint8_t x;
    uint8_t y;
};

struct textbox {
    const struct coordinate start;
    const struct coordinate end;
    struct coordinate cursor;
};

static inline void newline(struct textbox * box) {
    LCDfillrect(box->cursor.x, box->cursor.y, box->end.x - box->cursor.x, 8, WHITE); // Okay, this only makes sense for scrolling text. But whatever.
    box->cursor.x += box->end.x;
}
static inline void display_char_within_box(struct textbox * box, const char * data) {
    if (*data == '\0' || *data == '\n') {
        if (box->cursor.x != box->start.x)
            newline(box);
    } else {
        LCDdrawchar(box->cursor.x, box->cursor.y, *data);
        box->cursor.x += 6;
    }
    if (box->cursor.x > (box->end.x - 5)) {
        box->cursor.x = box->start.x;
        box->cursor.y += 8;
    }
    if (box->cursor.y > (box->end.y - 7))
        box->cursor.y = box->start.y;
}
static inline void display_text_within_box(struct textbox * box, char * data) {
    while (*data == '\n')
        newline(box), data++;

    do {
        display_char_within_box(box, data);
    } while (*data++ != '\0');
    LCDdisplay();
}

static inline void display_text_at_pos(int x, int y, char * data) {
    struct textbox box = { {x, y}, {x%8?LCDWIDTH-(7-(x%8)):LCDWIDTH, y%6?LCDHEIGHT-(5-(y%6)):LCDHEIGHT}, {x, y} };
    display_text_within_box(&box, data);
}
static inline void display_text(char * data) {
    display_text_at_pos(0, 0, data);
}

inline void lcd_init(void) {
    LCDInit(LCD_SCLK, LCD_DIN, LCD_DC, LCD_CS, LCD_RST, LCD_CONTRAST);
    pinMode(LCD_CS, OUTPUT);
    LCDclear();
}

inline void lcd_set_backlight_state(bool value) {
    if (value)
        digitalWrite(LCD_BL, HIGH);
    else
        digitalWrite(LCD_BL, LOW);
}

inline void lcd_dialog(const char * format, ...) {
    LCDclear();
    LCDdrawrect(5, 10, LCDWIDTH-10, LCDHEIGHT-20, BLACK);
    struct textbox box = { {15, 10}, {LCDWIDTH-15, LCDHEIGHT-10}, {15, 10} };
    char c[85];
    va_list args;
    va_start(args, format);
    vsnprintf(c, 85, format, args);
    va_end(args);
    display_text_within_box(&box, c);
}

inline void lcd_write_text(uint8_t x, uint8_t y, const char * format, ...) {
    LCDclear();
    struct textbox box = { {x*6, y*8}, {LCDWIDTH-(x*6), LCDHEIGHT-(y*8)}, {x*6, y*8} };
    char c[85];
    va_list args;
    va_start(args, format);
    vsnprintf(c, 85, format, args);
    va_end(args);
    display_text_within_box(&box, c);
}
