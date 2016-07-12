@echo off
set PATH="C:\Program Files\Arduino\hardware\tools\avr\bin";%PATH%

:loop
del test.*
make
if exist test.hex goto usb

:miss
pause
cls
goto loop

:usb
..\tool\reset COM10
make usb
avr-size test.elf -C --mcu=atmega32u4

pause
cls
goto loop
