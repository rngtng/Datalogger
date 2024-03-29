#
# Makefile for an Arduino based -injector- using the 'arduino-core'
# package and makefiles.
#
#
TARGET                 = datalogger
ARDUINO_LIBS           = ByteBuffer TimerOne EEPROM SD SD/utility
ARDUINO_DIR            = /Applications/Arduino.app/Contents/Resources/Java
AVRDUDE_CONF           = /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/etc/avrdude.conf
ARDUINO_LIB_PATH       = ../libraries /Applications/Arduino.app/Contents/Resources/Java/libraries

#MCU                   = atmega1280
MCU                    = atmega328p
F_CPU                  = 16000000

# That'S for Mac OS X
ARDUINO_PORT           = /dev/tty.usb*
AVRDUDE_ARD_PROGRAMMER = arduino
AVRDUDE_ARD_BAUDRATE   = 57600

include ../Arduino.mk/Arduino.mk

