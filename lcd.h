#include <stdbool.h>

// Pin references -> GPIO numbers
#ifndef LCD_SCLK
#define LCD_SCLK 0
#endif
#ifndef LCD_DIN
#define LCD_DIN 1
#endif
#ifndef LCD_DC
#define LCD_DC 2
#endif
#ifndef LCD_CS
#define LCD_CS 3
#endif
#ifndef LCD_RST
#define LCD_RST 4
#endif
#ifndef LCD_BL
#define LCD_BL 5
#endif

#ifndef LCD_CONTRAST
#define LCD_CONTRAST 0x3F
#endif

inline void lcd_init(void);
inline void lcd_dialog(const char *, ...);
inline void lcd_write_text(uint8_t x, uint8_t y, const char *, ...);
inline void lcd_set_backlight_state(bool);
