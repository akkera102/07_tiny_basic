@echo off
SET VAL1="C:\Program Files\Arduino\hardware\tools\avr\bin"
SET VAL2=COM10
SET VAL3="C:\Program Files\Arduino\hardware\tools\avr\etc\avrdude.conf"
SET VAL4=COM9

set PATH=%VAL1%;%PATH%


tool\reset %VAL2%
avrdude -C%VAL3% -v -patmega32u4 -cavr109 -P%VAL4% -b57600 -D -Uflash:w:test.hex:i

pause
