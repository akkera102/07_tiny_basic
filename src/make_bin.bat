@echo off
set PATH="C:\Program Files (x86)\Arduino\hardware\tools\avr\bin";%PATH%

:loop
del test.*

make
avr-size test.elf -C --mcu=atmega32u4

pause
cls
goto loop
