###############################################################################
#
# Øving 1 UCSysDes Makefile
#
###############################################################################

# assembler
AS = avr32-as
# assembleropsjoner
ASFLAGS = -gstabs

# linker
LD = avr32-ld
# linkeropsjoner
LDFLAGS = 

# flashprogrammerer
AVR32PROGRAM = avr32program

###############################################################################
# make-regler

# link: lag ELF fra objektfiler
interrupt.elf: interrupt.o
	$(LD) $(LDFLAGS) $^ -o $@

# assembly: lag objektfil fra assemblyfil
interrupt.o: interrupt.s io.s
	$(AS) $(ASFLAGS) -o $@ $<

# last opp til STK1000 (programmere flash)
.PHONY : upload
upload : interrupt.elf
	$(AVR32PROGRAM) halt
	sleep 3
	$(AVR32PROGRAM) program -e -f0,8Mb $<

# fjern alle autogenererte filer
.PHONY : clean
clean :
	rm -rf *.o *.elf
