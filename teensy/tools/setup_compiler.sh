#!/bin/sh

tar xvJf arm.tar.xz
tar xvJf arm-none-eabi.tar.xz -C arm
sh setup_segger.sh
