ARDMK_DIR = /usr

ARDUINO_DIR = /usr/share/arduino

BOARD_TAG    = mega2560
ARDUINO_PORT = /dev/ttyUSB1

ARDUINO_LIBS = SPI

.PHONY: intercept

intercept:
	./build all

include /usr/share/arduino/Arduino.mk
