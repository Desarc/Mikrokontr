###############################################################################
#
# Linux kjernemodul Makefile
#
###############################################################################

# navn på kjernemodul (navnet på C-fila men uten ".c")
MODULE = screen
ADDR = 129.241.103.251:/root


default:
	avr32-linux-gcc ${MODULE}.c -o ${MODULE}

	
.PHONY : send
send:
	$(shell scp ${MODULE} root@${ADDR})

.PHONY : sendimages
sendimages:
	$(shell scp *.bmp root@${ADDR})


.PHONY : clean
clean:
	rm -rf *.o

