# Arduino SDK
Programming Arduino/Teensy in pure C or C++ 

## Packages to install (Teensy)

  - arm-none-eabi-gcc-g++


## How to use (Teensy)

```bash
$ make
$ make upload
$ make blink
```

## Packages to install (Arduino)

  - To be able to compile and upload C code to an Arduino board, one has to install the following packages:
    - avr-gcc : Cross Compiling GNU GCC targeted at avr
    - avr-libc : C library for use with GCC on Atmel AVR microcontrollers
    - avrdude : Software for programming Atmel AVR microcontroller


## How to use (Arduino)

```bash
$ sh create_arduino_sdk.sh
...
$ sh create_arduino_project.sh led
```


## Reference

 - https://balau82.wordpress.com/2011/03/29/programming-arduino-uno-in-pure-c/
 - http://www.florentflament.com/blog/arduino-hello-world-without-ide.html
 - https://github.com/apmorton/teensy-template
 - https://github.com/PaulStoffregen/teensy_loader_cli

