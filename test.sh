#!/bin/bash

export avrdudeconf=/Applications/Arduino.app/Contents/Java/hardware/tools/avr/etc/avrdude.conf
export uart=/dev/tty.usbmodem1421
export hexfile=test_arduboy_10.hex

python -c "import serial;ser=serial.Serial('/dev/tty.usbmodem1421',1200);ser.write('');ser.close()"
sleep 2
avrdude -C${avrdudeconf} -v -patmega32u4 -cavr109 -P${uart} -b57600 -D -Uflash:w:${hexfile}:i
