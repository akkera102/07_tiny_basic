#!/bin/bash

export uart=/dev/tty.usbserial

if [ `uname -s` = "Darwin" ]; then
	export avrdudeconf=/Applications/Arduino.app/Contents/Java/hardware/tools/avr/etc/avrdude.conf
	export uart=/dev/tty.usbmodem1421
	python -c "import serial;ser=serial.Serial('/dev/tty.usbmodem1421',1200);ser.write('');ser.close()"
else
	export avrdudeconf=/usr/share/arduino/hardware/tools/avrdude.conf
	if [ ! -f $avrdudeconf ]; then
		export avrdudeconf=/etc/avrdude.conf
		if ! [ -f $avrdudeconf ]; then
			echo "Couldn't find avrdudeconf - make sure you have arduino IDE installed and edit this file to provide the path"
			exit
		fi
	fi
	python -c "import serial;import sys;ser=serial.Serial(sys.argv[1],1200);ser.write('');ser.close()" $uart
fi

export hexfile=test.hex

sleep 2
avrdude -C${avrdudeconf} -v -patmega32u4 -cavr109 -P${uart} -b57600 -D -Uflash:w:${hexfile}:i

