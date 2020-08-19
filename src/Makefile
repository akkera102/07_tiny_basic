MAKEFLAGS	+=--no-print-directory

BINPATH			= "C:/Program Files (x86)/Arduino/hardware/tools/avr/bin/"
CONFFILE		= "C:/Program Files (x86)/Arduino/hardware/tools/avr/etc/avrdude.conf"
COM				= COM3

ARDUBOY_MODEL	= ARDUBOY_10
#ARDUBOY_MODEL	= ARDUBOY_DEVKIT

#=============================================================================

PREFIX		= $(BINPATH)avr-
CPP			= $(PREFIX)g++
CC 			= $(PREFIX)gcc
AS			= $(PREFIX)gcc
AR 			= $(PREFIX)ar
LD 			= $(PREFIX)gcc
OBJCOPY		= $(PREFIX)objcopy
NM			= $(PREFIX)nm

SFILES	= \
	$(wildcard core/*.s) \
	$(wildcard core/*.S) \

CFILES	= \
	$(wildcard core/*.c) \
	$(wildcard core/*.cpp) \
	$(wildcard lib/*.c) \
	$(wildcard lib/*.cpp) \
	$(wildcard *.c) \
	$(wildcard *.cpp) \

VPATH		= $(dir $(CFILES) $(SFILES))
INCDIR		= core/

OBJDIR		= obj
NAME		= test
MAPFILE		= $(NAME).map
NMFILE      = $(NAME).nm
TARGET_ELF	= $(NAME).elf
TARGET_HEX	= $(NAME).hex

#=============================================================================

DFLAGS	= -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_LEONARDO -DARDUINO_ARCH_AVR -DUSB_VID=0x2341 -DUSB_PID=0x8036 -DUSB_MANUFACTURER="Unknown" -D$(ARDUBOY_MODEL)

GFLAGS	= -c -g -Os -Wall -Wextra -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega32u4 $(DFLAGS) $(foreach incdir,$(INCDIR),-I$(incdir)) -x c++ -fpermissive

CFLAGS	= -c -g -Os -Wall -Wextra -std=gnu11 -ffunction-sections -fdata-sections -MMD -mmcu=atmega32u4 $(DFLAGS) -DUSB_PRODUCT="Arduino Leonardo" $(foreach incdir,$(INCDIR),-I$(incdir)) -x c

ASFLAGS	= -c -g -x assembler-with-cpp -mmcu=atmega32u4 $(DFLAGS) -DUSB_PRODUCT="Arduino Leonardo" $(foreach incdir,$(INCDIR),-I$(incdir))

LDFLAGS	= -Wall -Wextra -Os -Wl,--gc-sections -Wl,-Map=$(MAPFILE) -mmcu=atmega32u4 -T core/avr5.xn

OFILES	= $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(SFILES) $(CFILES))))

DFILES	= $(addprefix $(OBJDIR)/, $(addsuffix .d, $(notdir $(CFILES))))

#=============================================================================

.PHONY: all usb
all: 
	@make $(TARGET_HEX)

usb:
	@$(BINPATH)avrdude -C$(CONFFILE) -v -patmega32u4 -cavr109 -P$(COM) -b57600 -D -Uflash:w:$(TARGET_HEX):i

#=============================================================================

$(TARGET_HEX): $(TARGET_ELF)
	@$(OBJCOPY) -O ihex -R .eeprom $< $@

$(TARGET_ELF): $(OBJDIR) $(OFILES) Makefile
	@echo # Linking $@
	@$(AR) rcs $(OBJDIR)/core.a $(OFILES)
	@$(LD) $(LDFLAGS) -o $@ $(OBJDIR)/core.a
	@$(NM) -n $@ > $(NMFILE)

$(OBJDIR):
	@mkdir $@

#=============================================================================

.SUFFIXES: .S .s .cpp .c .o .a .d

$(OBJDIR)/%.cpp.o: %.cpp
	@echo # compiling $<
	@$(CPP) $(GFLAGS) $< -o $@

$(OBJDIR)/%.c.o: %.c
	@echo # compiling $<
	@$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/%.s.o: %.s
	@echo # assembling $<
	@$(AS) $(ASFLAGS) $< -o $@

$(OBJDIR)/%.S.o: %.S
	@echo # assembling $<
	@$(AS) $(ASFLAGS) $< -o $@


-include $(DFILES)
