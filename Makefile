CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -g -mmcu=${BOARD} -Wall -Os -DF_CPU=16000000UL
BOARD = atmega328p
PORT = /dev/ttyACM0
OBJ = lcd.o main.o ps2.o tty.o fs.o kernel.o shell.o commands.o

lumos.hex: lumos.elf
	${OBJCOPY} -j .text -j .data -O ihex lumos.elf lumos.hex

lumos.elf: $(OBJ)
	${CC} -mmcu=${BOARD} $(OBJ) -o lumos.elf

%.o: src/%.c
	${CC} ${CFLAGS} -c $< -o $@

flash:
	sudo avrdude -F -V -c arduino -p m328p -P ${PORT} -b 115200 -e -U flash:w:lumos.hex
	avr-size -C --mcu=atmega328p lumos.elf
	make clean

clean:
	$(RM) *.o *.elf *.hex
