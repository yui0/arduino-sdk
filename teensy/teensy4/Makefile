TARGET = $(notdir $(CURDIR))

#TEENSY_CORE_SPEED = 256000000
#TEENSY_CORE_SPEED = 396000000
TEENSY_CORE_SPEED = 600000000

CPUARCH = cortex-m7
DEFS = -D__IMXRT1062__ -DTEENSYDUINO=147 -DARDUINO=10809 -DF_CPU=$(TEENSY_CORE_SPEED) -DLAYOUT_US_ENGLISH -DUSB_SERIAL

BUILDDIR = /tmp/build
#COREPATH = teensy4
COREPATH = .
TOOLSPATH = $(CURDIR)/../tools
COMPILERPATH = $(TOOLSPATH)/arm/bin

SFLAGS = -x assembler-with-cpp
#FLAGS = -fomit-frame-pointer -fmodulo-sched -fmodulo-sched-allow-regmoves -flto -finline-functions -funswitch-loops -fpredictive-commoning -fgcse-after-reload -ftree-vectorize -fipa-cp-clone -fgraphite-identity -floop-block -floop-interchange -floop-strip-mine -ftree-loop-distribution -ftree-loop-linear
FLAGS = -fomit-frame-pointer -fmodulo-sched -fmodulo-sched-allow-regmoves -finline-functions -funswitch-loops -fpredictive-commoning -fgcse-after-reload -ftree-vectorize -fipa-cp-clone -fgraphite-identity -floop-block -floop-interchange -floop-strip-mine -ftree-loop-distribution -ftree-loop-linear
COMMONFLAGS = -mthumb -mcpu=$(CPUARCH) -mfloat-abi=hard -mfpu=fpv5-d16
SECTIONS = -ffunction-sections -fdata-sections
CPPFLAGS = -Wall -Ofast $(SECTIONS) $(FLAGS) -nostdlib -MMD $(COMMONFLAGS) $(DEFS) -Isrc -I$(COREPATH)
CXXFLAGS = -std=gnu++14 -fno-exceptions -fpermissive -fno-rtti -fno-threadsafe-statics -felide-constructors -Wno-error=narrowing
#LDFLAGS = -s -Wl,--gc-sections,--defsym=__rtc_localtime=0 --specs=nano.specs -nostdlib -nostartfiles
LDFLAGS = -s -Wl,--gc-sections,--relax,--print-memory-usage $(COMMONFLAGS) -T$(COREPATH)/imxrt1062.ld -flto
LIBS = -larm_cortexM7lfsp_math -lm -lstdc++

AS = $(abspath $(COMPILERPATH))/arm-none-eabi-as
CC = $(abspath $(COMPILERPATH))/arm-none-eabi-gcc
CXX = $(abspath $(COMPILERPATH))/arm-none-eabi-g++
OBJCOPY = $(abspath $(COMPILERPATH))/arm-none-eabi-objcopy
SIZE = $(abspath $(COMPILERPATH))/arm-none-eabi-size

# automatically create lists of the sources and objects
LC_FILES := $(wildcard $(LIBRARYPATH)/*/*.c)
LCPP_FILES := $(wildcard $(LIBRARYPATH)/*/*.cpp)
TC_FILES := $(wildcard $(COREPATH)/*.c)
TCPP_FILES := $(wildcard $(COREPATH)/*.cpp)
S_FILES := $(wildcard $(COREPATH)/*.S)
C_FILES := $(wildcard src/*.c)
CPP_FILES := $(wildcard src/*.cpp)
INO_FILES := $(wildcard src/*.ino)

# include paths for libraries
L_INC := $(foreach lib,$(filter %/, $(wildcard $(LIBRARYPATH)/*/)), -I$(lib))

SOURCES := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o) $(INO_FILES:.ino=.o) $(TC_FILES:.c=.o) $(TCPP_FILES:.cpp=.o) $(LC_FILES:.c=.o) $(LCPP_FILES:.cpp=.o) $(S_FILES:.S=.o)
OBJS := $(foreach src,$(SOURCES), $(BUILDDIR)/$(src))

all: hex

build: $(TARGET).elf

hex: $(TARGET).hex

post_compile: $(TARGET).hex
	@$(abspath $(TOOLSPATH))/teensy_loader_cli -mmcu=imxrt1062 -w -v "$(basename $<)".hex

reboot:
	@-$(abspath $(TOOLSPATH))/teensy_loader_cli -mmcu=imxrt1062 -r ../teensy_loader_cli_src/rebootor/rebootor.hex

#upload: post_compile reboot
upload: post_compile

$(BUILDDIR)/%.o: %.S
	@echo -e "[AS]\t$<"
	@mkdir -p "$(dir $@)"
	@$(CC) $(SFLAGS) $(CPPFLAGS) $(CFLAGS) $(L_INC) -o "$@" -c "$<"

$(BUILDDIR)/%.o: %.c
	@echo -e "[CC]\t$<"
	@mkdir -p "$(dir $@)"
	@$(CC) $(CPPFLAGS) $(CFLAGS) $(L_INC) -o "$@" -c "$<"

$(BUILDDIR)/%.o: %.cpp
	@echo -e "[CXX]\t$<"
	@mkdir -p "$(dir $@)"
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(L_INC) -o "$@" -c "$<"

$(BUILDDIR)/%.o: %.ino
	@echo -e "[CXX]\t$<"
	@mkdir -p "$(dir $@)"
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(L_INC) -o "$@" -x c++ -include Arduino.h -c "$<"

$(TARGET).elf: $(OBJS) $(LDSCRIPT)
	@echo -e "[LD]\t$@"
	@$(CC) $(LDFLAGS) -o "$@" $(OBJS) $(LIBS)

%.hex: %.elf
	@echo -e "[HEX]\t$@"
	@$(SIZE) "$<"
	@$(OBJCOPY) -O ihex -R .eeprom "$<" "$@"

# compiler generated dependency info
-include $(OBJS:.o=.d)

clean:
	@echo Cleaning...
	@rm -rf "$(BUILDDIR)"
	@rm -f "$(TARGET).elf" "$(TARGET).hex"
