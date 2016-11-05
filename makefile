ARDPATH  = /Applications/Arduino.app/Contents/Java
BINPATH  = $(ARDPATH)/hardware/tools/avr/bin
CONFFILE = $(ARDPATH)/hardware/tools/avr/etc/avrdude.conf
COM      = /dev/cu.usbmodem1411
PROTOCOL = avr109
SPEED    = 57600

ARDUBOY_MODEL = ARDUBOY_10
MCU_NAME      = atmega32u4
DEV_NAME      = "Arduino Leonardo"

#=============================================================================

PREFIX  = $(BINPATH)/avr-
CPP	    = $(PREFIX)g++
CC      = $(PREFIX)gcc
AR      = $(PREFIX)ar
LD      = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
NM      = $(PREFIX)nm

SRCDIR  = src
COREDIR = $(SRCDIR)/core
LIBDIR  = $(SRCDIR)/lib

CFILES = \
	$(wildcard $(SRCDIR)/*.c) \
	$(wildcard $(SRCDIR)/*.cpp) \
	$(wildcard $(SRCDIR)/**/*.c) \
	$(wildcard $(SRCDIR)/**/*.cpp) \

VPATH  = $(dir $(CFILES))
INCDIR = $(COREDIR)/

OBJDIR     = obj
OUTDIR     = bin
NAME       = test
MAPFILE    = $(OUTDIR)/$(NAME).map
NMFILE     = $(OUTDIR)/$(NAME).nm
TARGET_ELF = $(OUTDIR)/$(NAME).elf
TARGET_HEX = $(OUTDIR)/$(NAME).hex

#=============================================================================

DFLAGS = \
    -DF_CPU=16000000L \
    -DARDUINO=10606 \
    -DARDUINO_AVR_LEONARDO \
    -DARDUINO_ARCH_AVR \
    -DUSB_VID=0x2341 \
    -DUSB_PID=0x8036 \
    -DUSB_MANUFACTURER="Unknown" \
    -D$(ARDUBOY_MODEL)

GFLAGS = \
    -c \
    -g \
    -Os \
    -Wall \
    -Wextra \
    -std=gnu++11 \
    -fno-exceptions \
    -ffunction-sections \
    -fdata-sections \
    -fno-threadsafe-statics \
    -MMD \
    -mmcu=$(MCU_NAME) \
    $(DFLAGS) \
    $(foreach incdir,$(INCDIR),-I$(incdir)) \
    -x c++

CFLAGS = \
    -c \
    -g \
    -Os \
    -Wall \
    -Wextra \
    -std=gnu11 \
    -ffunction-sections \
    -fdata-sections \
    -MMD \
    -mmcu=$(MCU_NAME) \
    $(DFLAGS) \
    -DUSB_PRODUCT=$(DEV_NAME) \
    $(foreach incdir,$(INCDIR),-I$(incdir)) \
    -x c

LDFLAGS = \
    -Wall \
    -Wextra \
    -Os \
    -Wl,--gc-sections \
    -Wl,-Map=$(MAPFILE) \
    -mmcu=$(MCU_NAME)

OFILES = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(CFILES))))

DFILES = $(addprefix $(OBJDIR)/, $(addsuffix .d, $(notdir $(CFILES))))

#=============================================================================

.PHONY: all usb clean

all:
	@make $(TARGET_HEX)

size: all
	$(BINPATH)/avr-size $(TARGET_ELF) -C --mcu=$(MCU_NAME)

usb:
	@python tool/reset.py $(COM)
	@$(BINPATH)/avrdude -C$(CONFFILE) -p$(MCU_NAME) -c$(PROTOCOL) -P$(COM) -b$(SPEED) -D -Uflash:w:$(TARGET_HEX):i

clean:
	@rm -rf $(OBJDIR) $(OUTDIR) $(MAPFILE) $(NMFILE) $(TARGET_ELF) $(TARGET_HEX)

#=============================================================================

$(TARGET_HEX): $(TARGET_ELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

$(TARGET_ELF): $(OBJDIR) $(OUTDIR) $(OFILES) Makefile
	$(AR) rcs $(OBJDIR)/core.a $(OFILES)
	$(LD) $(LDFLAGS) -o $@ $(OBJDIR)/core.a
	$(NM) -n $@ > $(NMFILE)

$(OBJDIR):
	mkdir $@

$(OUTDIR):
	mkdir $@

#=============================================================================

.SUFFIXES: .cpp .c .o .a .d

$(OBJDIR)/%.cpp.o: %.cpp
	$(CPP) $(GFLAGS) $< -o $@

$(OBJDIR)/%.c.o: %.c
	$(CC) $(CFLAGS) $< -o $@

-include $(DFILES)
