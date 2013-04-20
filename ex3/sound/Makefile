###############################################################################
#
# Linux kjernemodul Makefile
#
###############################################################################

# navn på kjernemodul (navnet på C-fila men uten ".c")
MODULE = sound
ADDR = 129.241.102.251:/root


default:
	avr32-linux-gcc ${MODULE}.c -o ${MODULE} -lm

	
.PHONY : send
send:
	$(shell scp ${MODULE} root@${ADDR})


.PHONY : clean
clean:
	rm -rf *.o

