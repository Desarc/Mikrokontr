###############################################################################
#
# Linux kjernemodul Makefile
#
###############################################################################

# navn på kjernemodul (navnet på C-fila men uten ".c")
MODULE = sound
ADDR = 129.241.103.209:/root

default:
	rm -rf *.o
	avr32-linux-gcc ${MODULE}.c -o ${MODULE} -lm
	
.PHONY : send
send:
	$(shell scp ${MODULE} root@${ADDR})

.PHONY : sendsounds
sendsounds:
	$(shell scp *.wav root@${ADDR})

.PHONY : clean
clean:
	rm -rf *.o
