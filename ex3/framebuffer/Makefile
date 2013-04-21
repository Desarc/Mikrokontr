###############################################################################
#
# Linux kjernemodul Makefile
#
###############################################################################

# navn på kjernemodul (navnet på C-fila men uten ".c")
MODULE = screen
ADDR = 129.241.103.120:/root


default:
	avr32-linux-gcc ${MODULE}.c -o ${MODULE}

	
.PHONY : send
send:
	$(shell scp ${MODULE} root@${ADDR})


.PHONY : clean
clean:
	rm -rf *.o

