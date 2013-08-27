#include <SPI.h>

#include "ascii.h"
#include "big_numbers.h"
#include "pcd8544.h"

#define pcd8544_set_is_data() digitalWrite(PCD8544_PIN_DC, 1)
#define pcd8544_set_is_command() digitalWrite(PCD8544_PIN_DC, 0)

static uint8_t current_x = 0;
static uint8_t current_line = 0;

static inline void inc_x(uint8_t count) {
    current_x += count;
    if (current_x >= PCD8544_WIDTH) {
        current_line += current_x / PCD8544_WIDTH;
        current_x %= PCD8544_WIDTH;
        if (current_line >= PCD8544_LINES)
            current_line %= PCD8544_LINES;
    }
}

static inline void inc_col(void) {
    inc_x(6);
}

void pcd8544_init(void) {
    if (PCD8544_PIN_RST != 0) {
        pinMode(PCD8544_PIN_RST, OUTPUT);
        digitalWrite(PCD8544_PIN_RST, 0);
        delay(1);
        digitalWrite(PCD8544_PIN_RST, 1);
    }
    pinMode(PCD8544_PIN_BL, OUTPUT);
    pinMode(PCD8544_PIN_DC, OUTPUT);
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);

    pcd8544_set_is_command();
    SPI.transfer(PCD8544_FUNCTION_SET | PCD8544_FUNCTION_H);
    SPI.transfer(PCD8544_SET_CONTRAST | PCD8544_CONTRAST);
    SPI.transfer(PCD8544_TEMP_CONTROL);
    SPI.transfer(PCD8544_BIAS | PCD8544_BIAS_BS1 | PCD8544_BIAS_BS0);
    SPI.transfer(PCD8544_FUNCTION_SET);
    SPI.transfer(PCD8544_DISPLAY_CONTROL | PCD8544_DISPLAY_CONTROL_NORMAL_MODE);
    pcd8544_set_is_data();
}

void pcd8544_place_cursor_precise(uint8_t x, uint8_t line) {
    if (line >= PCD8544_LINES)
        line %= PCD8544_LINES;
    if (x >= PCD8544_WIDTH)
        x %= PCD8544_WIDTH;
    // Yes, x is pixel addressed, y is line addressed. This makes sense because
    // we write 8 pixels at a time.
    current_x = x;
    current_line = line;
    pcd8544_set_is_command();
    SPI.transfer(PCD8544_SET_X_ADDRESS | (current_x));
    SPI.transfer(PCD8544_SET_LINE_ADDRESS | current_line);
    pcd8544_set_is_data();
}

void pcd8544_place_cursor(uint8_t column, uint8_t line) {
    pcd8544_place_cursor_precise(column * 6, line);
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
    if (value == '\n') {
        pcd8544_newline();
        return;
    }
    if (value == '\0') {
        if (current_x != 0)
            pcd8544_newline();
        return;
    }
    const uint8_t * data = ASCII[value - ' '];
    write_bytes(data, 5);
    SPI.transfer(0x00); // 8 blank bits to separate chars
    inc_col();
}

void pcd8544_draw_big_clock(const char * clock) {
    pcd8544_place_cursor_precise(0, 1);
    for (uint32_t line = 0 ; line != 3 ; line++) {
        write_byte(0x00, 3);
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
        write_byte(0x00, 4);
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
    uint8_t n = PCD8544_WIDTH - current_x;
    for (uint8_t i = 0; i != n; i++)
        SPI.transfer(0x00);
    inc_x(n);
}
