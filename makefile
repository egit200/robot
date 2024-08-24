
CC=/usr/bin/avr-gcc
OBJCOPY=/usr/bin/avr-objcopy

PORT=/dev/ttyUSB0
MCU=atmega328p

#CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os -DF_CPU=16000000UL -std=gnu99 -ffunction-sections -fdata-sections
CFLAGS=-Wall -mmcu=$(MCU) -Os -DF_CPU=16000000UL -I./rtos  -fdata-sections -ffunction-sections
#LDFLAGS=-Wl,-gc-sections -Wl,-relax 
#LDFLAGS= -Wl,-relax 
LDFLAGS= 

RTOS=rtos/list.o rtos/tasks.o  rtos/port.o rtos/heap_3.o rtos/queue.o rtos/timers.o rtos/variantHooks.o
CTRL=battery.o  distance.o serial.o main.o panel.o

OBJECT_FILES= $(RTOS)  $(CTRL)

ex.hex: ex.elf 
	${OBJCOPY} -O ihex -R .eeprom ex.elf ex.hex

ex.elf: $(OBJECT_FILES)
	${CC} $(CFLAGS) $(LDFLAGS) $(OBJECT_FILES) -o ex.elf 

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

install: ex.hex 
	/usr/bin/avr-size -C  --mcu=atmega328p ex.elf
	/usr/bin/avr-objdump -D ex.elf > ex.S
	avrdude -F -V -c arduino -p ATMEGA328P -P ${PORT} -b 57600 -U flash:w:ex.hex

clean:
	rm -f $(OBJECT_FILES) ex.hex ex.elf ex.S
