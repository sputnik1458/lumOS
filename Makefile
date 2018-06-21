CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -g -mmcu=${BOARD} -Wall -Os -DF_CPU=16000000UL
BOARD = atmega328p
PORT = /dev/ttyACM0

flash: kernel.hex
	sudo avrdude -F -V -c arduino -p m328p -P ${PORT} -b 115200 -e -U flash:w:kernel.hex
	avr-size -C --mcu=atmega328p kernel.elf

kernel.hex: kernel.elf
	${OBJCOPY} -j .text -j .data -O ihex kernel.elf kernel.hex

kernel.elf: lcd.o main.o ps2.o
	${CC} -mmcu=${BOARD} lcd.o main.o ps2.o -o kernel.elf

%.o: src/%.c
	${CC} ${CFLAGS} -c $< -o $@

clean:
	$(RM) *.o *.elf *.hex
