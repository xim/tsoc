#include <stdbool.h>

inline void lcd_init(void);
inline void lcd_dialog(const char *, ...);
inline void lcd_write_text(uint8_t x, uint8_t y, const char *, ...);
inline void lcd_set_backlight_state(bool);
