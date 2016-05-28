@echo off
set PATH="C:\Program Files\Arduino\hardware\tools\avr\bin";%PATH%

rem --------------------
:loop
del test.*
make
if exist test.hex goto usb

rem --------------------
:miss
pause
cls
goto loop

rem --------------------
:usb
..\tool\reset COM10
make usb
avr-size test.elf -C --mcu=atmega32u4


pause
cls
goto loop
