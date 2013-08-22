#include <SPI.h>

#include "ascii.h"
#include "big_numbers.h"
#include "pcd8544.h"

#define set_is_data() digitalWrite(PCD8544_PIN_DC, 1)
#define set_is_command() digitalWrite(PCD8544_PIN_DC, 0)

static uint8_t current_col = 0;
static uint8_t current_line = 0;

static inline void inc_col(void) {
    if (++current_col >= PCD8544_COLS) {
        current_col = 0;
        if (++current_line >= PCD8544_LINES)
            current_line = 0;
    }
}

void pcd8544_init(void) {
    pinMode(PCD8544_PIN_BL, OUTPUT);
    pinMode(PCD8544_PIN_DC, OUTPUT);
    pinMode(PCD8544_PIN_RST, OUTPUT);
    pinMode(PCD8544_PIN_SSEL, OUTPUT);
    SPI.begin();

    digitalWrite(PCD8544_PIN_RST, 0);
    delay(1);
    digitalWrite(PCD8544_PIN_RST, 1);
    digitalWrite(PCD8544_PIN_SSEL, 0);

    set_is_command();
    SPI.transfer(PCD8544_FUNCTION_SET | PCD8544_FUNCTION_H);
    SPI.transfer(PCD8544_SET_CONTRAST | PCD8544_CONTRAST);
    SPI.transfer(PCD8544_TEMP_CONTROL);
    SPI.transfer(PCD8544_BIAS | PCD8544_BIAS_BS1 | PCD8544_BIAS_BS0);
    SPI.transfer(PCD8544_FUNCTION_SET);
    SPI.transfer(PCD8544_DISPLAY_CONTROL | PCD8544_DISPLAY_CONTROL_NORMAL_MODE);
    set_is_data();
}

void pcd8544_place_cursor(uint8_t column, uint8_t line) {
    if (line >= PCD8544_LINES)
        line %= PCD8544_LINES;
    if (column >= PCD8544_COLS)
        column %= PCD8544_COLS;
    // Yes, x is pixel addressed, y is line addressed This makes sense because
    // we write 8 pixels at a time.
    current_col = column;
    current_line = line;
    set_is_command();
    SPI.transfer(PCD8544_SET_X_ADDRESS | (current_col * 6));
    SPI.transfer(PCD8544_SET_LINE_ADDRESS | current_line);
    set_is_data();
}

static inline void write_bytes(const uint8_t * data, size_t count) {
    for (size_t i = 0 ; i != count ; i++)
        SPI.transfer(*(data + i));
}

static inline void write_byte(const uint8_t data, size_t count) {
    for (size_t i = 0 ; i != count ; i++)
        SPI.transfer(data);
}

void pcd8544_write_char(char value) {
    if (value == '\n' || value == '\0') {
        // TODO this may not handle all cases, e.g. when a '\0' is the 15th
        // char of a line...
        pcd8544_newline();
        return;
    }
    const uint8_t * data = ASCII[value - ' '];
    write_bytes(data, 5);
    SPI.transfer(0x00); // 8 blank bits to separate chars
    inc_col();
}

void pcd8544_draw_big_clock(const char * clock) {
    for (uint32_t line = 0 ; line != 3 ; line++) {
        pcd8544_place_cursor(4, line + 1);
        for (uint32_t i = 0; i != 5 ; i++)
            if ((*(clock + i)) == ':') {
                if (current_timestamp % 2)
                    write_bytes(colon_24x8[line], 8);
                else
                    write_byte(0x00, 8);
                write_byte(0x00, 1);
            } else {
                // The font data, index 24 * numerical value of the char
                write_bytes(numbers_24x16[line] + (16 * ((*(clock + i)) - '0')), 16);
                write_byte(0x00, 1);
            }
    }
}

void pcd8544_write_string(const char * data) {
    while (*data == '\n')
        pcd8544_newline(), data++;

    do
        pcd8544_write_char(*data);
    while (*data++ != '\0');
}

void pcd8544_clear(void) {
    for (uint16_t i = 0; i != (PCD8544_WIDTH * PCD8544_LINES); i++)
        SPI.transfer(0x00);
}

void pcd8544_newline(void) {
    while (current_col != 0) {
        for (uint8_t i = 0; i != 6; i++)
            SPI.transfer(0x00);
        inc_col();
    }
}
