#!/bin/sh

echo dnf install avr-gcc-c++ avr-libc avrdude

dir=arduino-lib
mkdir -p ${dir}/{include,lib,src}
#git init
#git config core.sparsecheckout true
#git remote add origin https://github.com/arduino/Arduino.git
#echo "hardware/arduino/avr/cores/arduino/" > .git/info/sparse-checkout
##git read-tree -m -u HEAD
#git pull origin master
#git clone --branch master --single-branch https://github.com/arduino/Arduino.git/hardware/arduino/avr/cores/arduino
svn checkout https://github.com/arduino/Arduino.git/trunk/hardware/arduino/avr/cores/arduino
svn checkout https://github.com/arduino/Arduino.git/trunk/hardware/arduino/avr/variants/standard
svn checkout https://github.com/arduino/Arduino.git/trunk/hardware/arduino/avr/libraries/HID/src

cp arduino/*.c ${dir}/src/
cp arduino/*.cpp ${dir}/src/
cp arduino/*.h ${dir}/include/
cp src/*.c* ${dir}/src/
cp src/*.h ${dir}/include/
#cp arduino/avr-libc/*.c ${dir}/src/
#cp arduino/avr-libc/*.cpp ${dir}/src/
#cp arduino/avr-libc/*.h ${dir}/include/
cp standard/*.h  ${dir}/include/
cp arduino/main.cpp _template_proj/
#rm -rf Arduino_git

make
make clean

rm -rf arduino src standard

