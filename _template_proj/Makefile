ASM=avr-as
CC=avr-gcc
CXX=avr-g++
INCLUDE=-I ../arduino-lib/include
LIBS=-L ../arduino-lib/lib -lm ../arduino-lib/lib/libarduino.a
MCU=-mmcu=atmega328p
CPU_SPEED=-DF_CPU=16000000UL
CFLAGS=$(MCU) $(CPU_SPEED) -Os -w -Wl,--gc-sections -ffunction-sections -fdata-sections

# FreeBSD
#PORT=/dev/cuaU0 # FreeBSD
PORT=/dev/ttyACM0

default: build

build: arduino_temp1.hex

arduino_temp1.hex: arduino_temp1.elf
	avr-objcopy -O ihex $< $@

OBJECTS= main.o arduino_temp1.o

arduino_temp1.elf: $(OBJECTS)
	$(CXX) $(CFLAGS) $(INCLUDE) $^ -o $@ $(LIBS)

upload:
	avrdude -V -F -p m328p -c arduino -b 115200 -Uflash:w:arduino_temp1.hex -P$(PORT)

clean:
	@echo -n Cleaning ...
	$(shell rm arduino_temp1.elf 2> /dev/null)
	$(shell rm arduino_temp1.hex 2> /dev/null)
	$(shell rm *.o 2> /dev/null)
	@echo " done"

%.o: %.s
	$(ASM) $< $(CFLAGS) $(INCLUDE) -c -o $@
%.o: %.c
	$(CC) $< $(CFLAGS) $(INCLUDE) -c -o $@
%.o: %.cpp
	$(CXX) $< $(CFLAGS) $(INCLUDE) -c -o $@

