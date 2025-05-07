SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
BOARD_LIB = $(SELF_DIR)/library
SOURCE_LIB = $(wildcard $(BOARD_LIB)/*.c)
CLOCK_RATE = 16000000L
CC = gcc
CFLAGS = -g -std=c99 -Wall -O0 -D_XOPEN_SOURCE=700

TARGETS = neural_test

all: $(TARGETS)

neural_test: neural_test.c
	$(CC) $(CFLAGS) -o $@ $^ -lm

sample: sample.c $(wildcard $(BOARD_LIB)/*.c)
	avr-gcc -I$(BOARD_LIB) -DF_CPU=$(CLOCK_RATE) -Wall -mmcu=atmega645a -O2 -o main.elf sample.c $(wildcard $(BOARD_LIB)/*.c)
	avr-objcopy -O ihex main.elf main.hex
	avr-size main.elf

neural: neural.c pid.c $(wildcard $(BOARD_LIB)/*.c)
	avr-gcc -I$(BOARD_LIB) -DF_CPU=$(CLOCK_RATE) -Wall -mmcu=atmega645a -O2 -o main.elf neural.c pid.c $(wildcard $(BOARD_LIB)/*.c)
	avr-objcopy -O ihex main.elf main.hex
	avr-size main.elf

neural_j: neural_j.c pid.c $(wildcard $(BOARD_LIB)/*.c)
	avr-gcc -I$(BOARD_LIB) -DF_CPU=$(CLOCK_RATE) -Wall -mmcu=atmega645a -O2 -o main.elf neural_j.c pid.c $(wildcard $(BOARD_LIB)/*.c)
	avr-objcopy -O ihex main.elf main.hex
	avr-size main.elf

test: pid_test.c pid.c $(wildcard $(BOARD_LIB)/*.c)
	avr-gcc -I$(BOARD_LIB) -DF_CPU=$(CLOCK_RATE) -Wall -mmcu=atmega645a -O2 -o main.elf pid_test.c pid.c $(wildcard $(BOARD_LIB)/*.c)
	avr-objcopy -O ihex main.elf main.hex
	avr-size main.elf

off: motorOff.c $(wildcard $(BOARD_LIB)/*.c)
	avr-gcc -I$(BOARD_LIB) -DF_CPU=$(CLOCK_RATE) -Wall -mmcu=atmega645a -O2 -o main.elf motorOff.c $(wildcard $(BOARD_LIB)/*.c)
	avr-objcopy -O ihex main.elf main.hex
	avr-size main.elf

lib: $(wildcard $(BOARD_LIB)/*.c)
	$(foreach var,$(SOURCE_LIB), \
        avr-gcc -I$(BOARD_LIB) -DF_CPU=$(CLOCK_RATE) -Wall -mmcu=atmega645a -O2 -c -o $(var:.c=.o) $(var); )

program_windows: 
	avrdude -F -pm645 -Pcom4 -carduino -u -U flash:w:main.hex

#edit the device to match the device on your computer
MAC_DEVICE = `ls /dev/cu*modem*`
program_mac: main.elf
	avrdude -pm645 -P $(MAC_DEVICE) -c arduino -F -u -U flash:w:main.hex

program_linux: main.elf
	avrdude -pm645 -P /dev/ttyACM0 -c arduino -F -u -U flash:w:main.hex

clean:
	rm -fr *.elf *.hex *.o
