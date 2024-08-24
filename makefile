#CC=/opt/avr-gcc-gnat-7.3.0/bin/avr-gcc
CC=/usr/bin/avr-gcc
OBJCOPY=/usr/bin/avr-objcopy

#CFLAGS=-Os -DF_CPU=16000000UL -mmcu=atmega328p
PORT=/dev/ttyUSB0
MCU=atmega328p
#CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os -DF_CPU=16000000UL -std=gnu99 -ffunction-sections -fdata-sections
CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os -DF_CPU=16000000UL -std=gnu99 -ffunction-sections -fdata-sections
#LDFLAGS=-Wl,-gc-sections -Wl,-relax 
LDFLAGS= -Wl,-relax 

CTRL=battery.o panel.o distance.o serial.o
OBJECT_FILES=list.o tasks.o  port.o heap_3.o queue.o timers.o main.o task-priority.o $(CTRL)
#OBJECT_FILES=main2.o serial.o 

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
