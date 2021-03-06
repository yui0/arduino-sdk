# Kazane
# ©2017-2020 Yuichiro Nakada

TARGET = kazane

MCU = IMXRT1062

LOWER_MCU := $(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$(MCU)))))))))))))))))))))))))))
MCU_LD = $(LOWER_MCU).ld

OPTIONS = -DF_CPU=800000000 -DUSB_AUDIO -DLAYOUT_US_ENGLISH -DUSING_MAKEFILE
OPTIONS += -D__$(MCU)__ -DARDUINO=10810 -DTEENSYDUINO=149 -DARDUINO_TEENSY40

CPUOPTIONS = -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16 -mthumb

SFLAGS = -x assembler-with-cpp
FLAGS = -fomit-frame-pointer -fmodulo-sched -fmodulo-sched-allow-regmoves -finline-functions -funswitch-loops -fpredictive-commoning -fgcse-after-reload -ftree-vectorize -fipa-cp-clone -fgraphite-identity -floop-block -floop-interchange -floop-strip-mine -ftree-loop-distribution -ftree-loop-linear -nostdlib
# -flto

CPPFLAGS = --specs=nano.specs -Wall -Os $(CPUOPTIONS) -MMD $(OPTIONS) -I. -ffunction-sections -fdata-sections $(FLAGS)
CXXFLAGS = -std=gnu++14 -felide-constructors -fno-exceptions -fpermissive -fno-rtti -Wno-error=narrowing
CFLAGS =
LDFLAGS = --specs=nano.specs -Wl,--gc-sections,--relax $(SPECS) $(CPUOPTIONS) -T$(MCU_LD)
LIBS = -larm_cortexM7lfsp_math -lm -lstdc++

BUILDDIR = /tmp/build
TOOLSPATH = $(CURDIR)/../tools
#LIBRARYPATH = 
COMPILERPATH = $(TOOLSPATH)/arm/bin

AS = $(abspath $(COMPILERPATH))/arm-none-eabi-as
CC = $(abspath $(COMPILERPATH))/arm-none-eabi-gcc
CXX = $(abspath $(COMPILERPATH))/arm-none-eabi-g++
OBJCOPY = $(abspath $(COMPILERPATH))/arm-none-eabi-objcopy
SIZE = $(abspath $(COMPILERPATH))/arm-none-eabi-size

S_FILES := $(wildcard *.S)
C_FILES := $(wildcard *.c)
CPP_FILES := $(wildcard *.cpp)
SOURCES := $(S_FILES:.S=.o) $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)
OBJS := $(foreach src,$(SOURCES), $(BUILDDIR)/$(src))
##OBJS := $(S_FILES:.S=.o) $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)

all: $(TARGET).hex

$(TARGET).elf: $(OBJS) $(MCU_LD)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

%.hex: %.elf
	$(SIZE) $<
	$(OBJCOPY) -O ihex -R .eeprom $< $@

post_compile: $(TARGET).hex
	@$(abspath $(TOOLSPATH))/teensy_loader_cli -mmcu=$(MCU) -w -v "$(basename $<)".hex

reboot:
	@-$(abspath $(TOOLSPATH))/teensy_loader_cli -mmcu=$(MCU) -r ../teensy_loader_cli_src/rebootor/rebootor.hex

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

# compiler generated dependency info
-include $(OBJS:.o=.d)

clean:
	@echo Cleaning...
	@rm -rf "$(BUILDDIR)"
	@rm -f "$(TARGET).elf" "$(TARGET).hex"
##	rm -f *.o *.d $(TARGET).elf $(TARGET).hex
