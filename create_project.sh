#!/bin/sh
cp -r _template_proj $1
mv $1/template.cpp $1/$1.cpp 
sed -e "s/arduino_temp1/$1/g" _template_proj/Makefile > $1/Makefile

