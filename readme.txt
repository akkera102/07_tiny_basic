TOYOSHIKI Tiny Basic for Arduboy - Interpreter

## Description
TOYOSHIKI Tiny Basic is Palo Alto Tiny BASIC(1976) clone.
Interpreter is magic. if you see internal structure, you will become wizard :)


## Installation
Request : Arduino IDE 1.6.x

1. download from github
2. pls edit test.bat(windows) or test.sh(osx) for your computer(must change COM port number)
3. execute test.bat
4. this program need Serial Monitor. i tested Tera Term. it is famous in Japan.
5. input it after connect.


## Manual
Tiny BASIC grammar see wikipedia or source code.
https://en.wikipedia.org/wiki/Tiny_BASIC


## Original
simple source code. thank you!
https://github.com/vintagechips/ttbasic_win


## ToDo
INPUT order.


## Development
compiler        : windows Arduino IDE 1.6.9(avr-gcc) + 1.0.6(make.exe)
image converter : python 2.6 + PIL
etc             : Visual Studio 2015 C#


## History
v1.00 2016/05/28    first version


Arduboy 1.0 infomation.

AVR Memory Usage
----------------
Device: atmega32u4

Program:   15444 bytes (47.1% Full)
(.text + .data + .bootloader)

Data:       1798 bytes (70.2% Full)
(.data + .bss + .noinit)
