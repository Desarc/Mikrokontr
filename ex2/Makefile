CC=avr32-gcc
CFLAGS=-g -Wall -Iinclude 
LDFLAGS=-g -Wall -pedantic -Llib -lavr32b-interrupts -lm
FILE=lights
TARGET=$(FILE).elf


# Command to upload program to flash 
AVR32PROGRAM = avr32program

####### RULES ##########################################################
.PHONY : all
all: $(TARGET)

$(FILE).elf: $(FILE).o
	$(CC) $^ -o $@ $(LDFLAGS)

$(FILE).o: $(FILE).c 
	$(CC) $(CFLAGS) -c $< -o $@

# upload to STK1000 (program flash)
.PHONY : upload
upload : $(TARGET)
	$(AVR32PROGRAM) halt
	sleep 3
	$(AVR32PROGRAM) program -e -f0,8Mb $<

.PHONY : clean
clean :
	rm -rf *.o *.elf *.*~
