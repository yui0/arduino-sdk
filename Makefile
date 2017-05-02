CC=avr-gcc
CXX=avr-g++
MCU=-mmcu=atmega328p 
CPU_SPEED=-DF_CPU=16000000UL # 16 Mhz
LIBNAME=arduino-lib/lib/libarduino.a
INCDIR=arduino-lib/include
CFLAGS=$(MCU) $(CPU_SPEED) -Os -w -I $(INCDIR)

OBJECTS=arduino-lib/src/CDC.o \
	arduino-lib/src/HID.o \
	arduino-lib/src/wiring.o \
	arduino-lib/src/IPAddress.o \
	arduino-lib/src/Stream.o \
	arduino-lib/src/USBCore.o \
	arduino-lib/src/new.o \
	arduino-lib/src/hooks.o \
	arduino-lib/src/wiring_analog.o \
	arduino-lib/src/wiring_digital.o \
	arduino-lib/src/wiring_pulse.o \
	arduino-lib/src/wiring_shift.o \
	arduino-lib/src/HardwareSerial.o \
	arduino-lib/src/Print.o \
	arduino-lib/src/Tone.o \
	arduino-lib/src/WMath.o \
	arduino-lib/src/WString.o \
	arduino-lib/src/WInterrupts.o \
#	arduino-lib/src/malloc.o \
#	arduino-lib/src/realloc.o

default: $(OBJECTS)
	avr-ar rcs $(LIBNAME) $^

%.o : %.c
	$(CC) $< $(CFLAGS) -c -o $@

%.o : %.cpp
	$(CXX) $< $(CFLAGS) -c -o $@

clean:
	rm -f arduino-lib/src/$(OBJECTS)
