# Zombie Survival Makefile
# Liam Diprose and Jeremy Craig
#
# TODO: Stuff about the makefile: targets etc
#


# Target file, without extention. i.e.  $(TARGET).c
TARGET = main

# Optimisation, s for compiled size optimisation
OPTIMISATION = s

# MCU to compile for/flash
MMCU = atmega32u2

# Definitions.
CC = avr-gcc
CFLAGS += -mmcu=$(MMCU)
CFLAGS += -O$(OPTIMISATION)
CFLAGS += -Wall
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wextra -g -I. -Idrivers -Iutils

OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: $(TARGET).out


# Compile: create object files from C source files.
$(TARGET).o: $(TARGET).c drivers/system.h drivers/led.h drivers/display.h utils/pacer.h utils/task.h
	$(CC) -c $(CFLAGS) $< -o $@

pio.o: drivers/pio.c drivers/pio.h drivers/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: drivers/system.c drivers/system.h
	$(CC) -c $(CFLAGS) $< -o $@

timer.o: drivers/timer.c drivers/system.h drivers/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

led.o: drivers/led.c drivers/pio.h drivers/system.h drivers/led.h
	$(CC) -c $(CFLAGS) $< -o $@

pacer.o: utils/pacer.c drivers/system.h drivers/timer.h utils/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@

display.o: drivers/display.c drivers/system.h drivers/display.h drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

ledmat.o: drivers/ledmat.c drivers/pio.h drivers/system.h drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

task.o: utils/task.c drivers/system.h drivers/timer.h utils/task.h
	$(CC) -c $(CFLAGS) $< -o $@

ir_uart.o: drivers/ir_uart.c drivers/ir_uart.h drivers/pio.h drivers/system.h drivers/timer0.h drivers/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

usart1.o: drivers/usart1.c drivers/system.h drivers/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

prescale.o: drivers/prescale.c drivers/prescale.h drivers/system.h
	$(CC) -c $(CFLAGS) $< -o $@

timer0.o: drivers/timer0.c drivers/bits.h drivers/prescale.h drivers/system.h drivers/timer0.h
	$(CC) -c $(CFLAGS) $< -o $@





# Link: create output file (executable) from object files.
main.out: $(TARGET).o pio.o system.o timer.o led.o pacer.o display.o ledmat.o task.o ir_uart.o usart1.o prescale.o timer0.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Create hex file for programming from executable file.
$(TARGET).hex: $(TARGET).out
	$(OBJCOPY) -O ihex $(TARGET).out $(TARGET).hex


# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: $(TARGET).hex
	dfu-programmer $(MMCU) erase --force; dfu-programmer $(MMCU) flash $(TARGET).hex; dfu-programmer $(MMCU) start


