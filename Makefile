# Author: Bruce Fletcher
# License: http://creativecommons.org/licenses/by/3.0/
# Makefile based on template from Crosspack-AVR: http://www.obdev.at/products/crosspack/index.html

###
# Stuff you might want to mess with:

PROGNAME   = badges
DEVICE     = atmega168p
CLOCK      = 8000000
#CLOCK      = 1000000
PROGRAMMER = -c avrispmkII -P usb
OBJECTS    = main.o timer.o uart.o matrix.o buttons.o banner.o breakout.o
#FUSES      = -U lfuse:w:0xd6:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
FUSES      = -U lfuse:w:0x62:m -U hfuse:w:0xdf:m -U efuse:w:0x01:m

###
# Stuff you probably don't want to mess with:

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
CC      = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -fshort-enums
CPP     = avr-g++ -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# symbolic targets:
all:	$(PROGNAME).hex

.cpp.o:
	$(CPP) -c $< -o $@

.c.o:
	$(CC) -c $< -o $@

.S.o:
	$(CC) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(CC) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:$(PROGNAME).hex:i

fuse:
	$(AVRDUDE) $(FUSES)

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash 

clean:
	rm -f $(PROGNAME).hex $(PROGNAME).elf $(OBJECTS)

# file targets:
$(PROGNAME).elf: $(OBJECTS)
	$(CC) -o $(PROGNAME).elf $(OBJECTS)

$(PROGNAME).hex: $(PROGNAME).elf
	rm -f $(PROGNAME).hex
	avr-objcopy -j .text -j .data -O ihex $(PROGNAME).elf $(PROGNAME).hex
	avr-size --format=avr --mcu=$(DEVICE) $(PROGNAME).elf
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	$(PROGNAME).elf
	avr-objdump -d $(PROGNAME).elf

cpp:
	$(CC) -E banner.c

