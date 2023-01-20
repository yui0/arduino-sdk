# Arduino SDK

[![Docker Hub](https://github.com/yui0/arduino-sdk/actions/workflows/build-and-push-image.yml/badge.svg)](https://github.com/yui0/arduino-sdk/actions/workflows/build-and-push-image.yml)
[![GPL License](https://img.shields.io/badge/license-GPL-blue.svg?style=flat)](LICENSE)

Development environment for Teensy in pure C or C++

Reference: [https://yui0.github.io/teensy4/](https://yui0.github.io/teensy4/)

## Packages to install (Teensy)

  - arm-none-eabi-gcc
  - arm-none-eabi-g++
  - [Embedded Studio for ARM (Optional)](https://www.segger.com/downloads/embedded-studio)

## How to use (Teensy)

```bash
$ make
$ make upload
$ make blink
```

## How to use with docker (Teensy)

```bash
$ docker pull berry0/teensy-dev:latest
$ docker run --net=host -it -d --name teensy-dev berry0/teensy-dev:latest
$ docker exec -it teensy-dev /bin/bash
# cd /arduino-sdk/teensy/tools/
# echo "yes" | sh setup_segger.sh
# cd /arduino-sdk/teensy/teensy4
# make
# make upload
# make clean
# cd /arduino-sdk/teensy/teensy4.mini
# make
# make upload
# make clean
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

