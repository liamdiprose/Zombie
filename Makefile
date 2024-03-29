# ----------------modfied makefile details----------------------
# File:   Makefile
# Author: Liam Diprose and Jeremy Craig
# Descr:  Makefile for Zombie Survival Makefile

# ----------------original makefile details----------------------
# Original Author: M. P. Hayes, UCECE
# Date:   12 Sep 2010



# Target file, without extention. i.e.  $(TARGET).c
TARGET = main

# Optimisation, s for compiled size optimisation
OPTIMISATION = fast

# MCU to compile for/flash
MMCU = atmega32u2

# C Standard
CSTD = c99

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=$(MMCU) -O$(OPTIMISATION)  -Wall -Wstrict-prototypes -Wextra -g -Idrivers -Igame -std=$(CSTD) -lm
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: $(TARGET).out program


# Compile: create object files from C source files.
main.o: main.c drivers/system.h drivers/led.h game/draw.h drivers/navswitch.h drivers/pacer.h game/player.h game/point.h game/event_manager.h game/communication.h game/protocol.h
	$(CC) -c $(CFLAGS) $< -o $@

pio.o: drivers/pio.c drivers/pio.h drivers/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: drivers/system.c drivers/system.h
	$(CC) -c $(CFLAGS) $< -o $@

timer.o: drivers/timer.c drivers/system.h drivers/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

led.o: drivers/led.c drivers/pio.h drivers/system.h drivers/led.h
	$(CC) -c $(CFLAGS) $< -o $@

pacer.o: drivers/pacer.c drivers/system.h drivers/timer.h drivers/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@

draw.o: game/draw.c drivers/system.h game/level.h game/point.h game/draw.h game/player.h drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

ledmat.o: drivers/ledmat.c drivers/pio.h drivers/system.h drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

event_manager.o: game/event_manager.c drivers/system.h drivers/timer.h game/event_manager.h
	$(CC) -c $(CFLAGS) $< -o $@

ir_uart.o: drivers/ir_uart.c drivers/ir_uart.h drivers/pio.h drivers/system.h drivers/timer0.h drivers/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

navswitch.o: drivers/navswitch.c drivers/system.h drivers/delay.h drivers/pio.h drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

usart1.o: drivers/usart1.c drivers/system.h drivers/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

prescale.o: drivers/prescale.c drivers/prescale.h drivers/system.h
	$(CC) -c $(CFLAGS) $< -o $@

timer0.o: drivers/timer0.c drivers/bits.h drivers/prescale.h drivers/system.h drivers/timer0.h
	$(CC) -c $(CFLAGS) $< -o $@

heartbeat.o: game/heartbeat.c drivers/system.h drivers/led.h game/heartbeat.h
	$(CC) -c $(CFLAGS) $< -o $@

level.o: game/level.c drivers/system.h game/draw.h game/player.h game/level.h
	$(CC) -c $(CFLAGS) $< -o $@

player.o: game/player.c drivers/system.h drivers/navswitch.h game/protocol.h game/point.h game/player.h
	$(CC) -c $(CFLAGS) $< -o $@

communication.o: game/communication.c drivers/system.h drivers/navswitch.h game/point.h game/communication.h drivers/ir_uart.h drivers/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@

protocol.o: game/protocol.c drivers/system.h drivers/navswitch.h game/point.h game/communication.h drivers/ir_uart.h drivers/pacer.h game/protocol.h
	$(CC) -c $(CFLAGS) $< -o $@

# Link: create output file (executable) from object files.
main.out: main.o pio.o system.o timer.o led.o pacer.o draw.o ledmat.o event_manager.o ir_uart.o usart1.o prescale.o timer0.o heartbeat.o level.o player.o navswitch.o communication.o protocol.o
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
	dfu-programmer $(MMCU) erase; dfu-programmer $(MMCU) flash $(TARGET).hex; dfu-programmer $(MMCU) start
