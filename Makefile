ARDMK_DIR = /usr

ARDUINO_DIR = /usr/share/arduino

AVR_TOOLS_DIR = /usr

BOARD_TAG    = mega2560
ARDUINO_PORT = /dev/ttyUSB1

ARDUINO_LIBS = SPI

.PHONY: intercept

intercept:
	./build all

include /usr/share/arduino/Arduino.mk
