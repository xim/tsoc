#ifndef PCD8544_H
#define PCD8544_H

#include <inttypes.h>
#include <stdbool.h>

// The display spec
#define PCD8544_WIDTH 84
#define PCD8544_HEIGHT 48
#define PCD8544_LINES 6
#define PCD8544_COLS 14

// Default pin assignments. Note thet DIN and CLK are set by hardware SPI.
#ifndef PCD8544_PIN_DC
#define PCD8544_PIN_DC 0
#endif
#ifndef PCD8544_PIN_RST
#define PCD8544_PIN_RST 0
#endif
#ifndef PCD8544_PIN_SSEL
#define PCD8544_PIN_SSEL 0
#endif
#ifndef PCD8544_PIN_BL
#define PCD8544_PIN_BL 0
#endif

// Default contrast value
#ifndef PCD8544_CONTRAST
#define PCD8544_CONTRAST 0x3F
#endif

// LCD commands, Table 1, page 14
#define PCD8544_FUNCTION_SET (1<<5)
#define PCD8544_FUNCTION_PD  (1<<2)
#define PCD8544_FUNCTION_V   (1<<1)
#define PCD8544_FUNCTION_H   (1<<0)

// Normal instructions, H = 0
#define PCD8544_DISPLAY_CONTROL (1<<3)
#define PCD8544_DISPLAY_CONTROL_D (1<<2)
#define PCD8544_DISPLAY_CONTROL_E (1<<0)
#define PCD8544_DISPLAY_CONTROL_BLANK 0
#define PCD8544_DISPLAY_CONTROL_NORMAL_MODE  PCD8544_DISPLAY_CONTROL_D
#define PCD8544_DISPLAY_CONTROL_ALL_ON       PCD8544_DISPLAY_CONTROL_E
#define PCD8544_DISPLAY_CONTROL_INVERSE      (PCD8544_DISPLAY_CONTROL_D|PCD8544_DISPLAY_CONTROL_E)

#define PCD8544_SET_LINE_ADDRESS (1<<6)
#define PCD8544_LINE_ADRESS_MASK 0b111
#define PCD8544_SET_X_ADDRESS (1<<7)
#define PCD8544_X_ADRESS_MASK 0b01111111

// Extended instructions. H = 1
#define PCD8544_TEMP_CONTROL (1<<2)
#define PCD8544_TEMP_TC1     (1<<1)
#define PCD8544_TEMP_TC0     (1<<0)

#define PCD8544_BIAS     (1<<4)
#define PCD8544_BIAS_BS2 (1<<2)
#define PCD8544_BIAS_BS1 (1<<1)
#define PCD8544_BIAS_BS0 (1<<0)

#define PCD8544_VOP (1<<7)
#define PCD8544_SET_CONTRAST PCD8544_VOP

inline void pcd8544_init(void);
inline void pcd8544_clear(void);
inline void pcd8544_place_cursor(uint8_t column, uint8_t row);
inline void pcd8544_write_char(char);
inline void pcd8544_write_string(const char *);
inline void pcd8544_newline();
inline void pcd8544_set_backlight_state(bool);
#endif
