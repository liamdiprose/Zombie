
# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Ofast -Wall -Wstrict-prototypes -Wextra -g -Idrivers -Igame
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: main.out


# Compile: create object files from C source files.
main.o: main.c drivers/system.h drivers/led.h game/draw.h drivers/navswitch.h drivers/pacer.h game/player.h game/point.h game/event_manager.h
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

player.o: game/player.c drivers/system.h drivers/navswitch.h game/point.h game/player.h
	$(CC) -c $(CFLAGS) $< -o $@




# Link: create output file (executable) from object files.
main.out: main.o pio.o system.o timer.o led.o pacer.o draw.o ledmat.o event_manager.o ir_uart.o usart1.o prescale.o timer0.o heartbeat.o level.o player.o navswitch.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Create hex file for programming from executable file.
main.hex: main.out
	$(OBJCOPY) -O ihex main.out main.hex


# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: main.hex
	dfu-programmer atmega32u2 erase --force; dfu-programmer atmega32u2 flash main.hex; dfu-programmer atmega32u2 start


