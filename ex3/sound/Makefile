###############################################################################
#
# Linux kjernemodul Makefile
#
###############################################################################

# navn på kjernemodul (navnet på C-fila men uten ".c")
MODULE = sound
ADDR = 129.241.103.248:/root


default:
	gcc ${MODULE}.c -o ${MODULE}

	
.PHONY : send
send:
	$(shell scp ${MODULE}.ko root@${ADDR})


.PHONY : clean
clean:
	rm -rf *.o

