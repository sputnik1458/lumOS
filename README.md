# lumOS
A lightweight operating system for AVR microcontrollers

## Features
* PS/2 keyboard driver
* [LCD display driver](http://tinyurl.com/peterfleury)
* Terminal
* EEPROM File System

## Usage
Modify values in the Makefile and header files for your specific microcontroller and circuit (i.e. clock speed, pins, interrupts, port).

Run `sudo make` to compile and `make flash` to install.
