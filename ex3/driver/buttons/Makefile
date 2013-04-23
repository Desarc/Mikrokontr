###############################################################################
#
# Linux kjernemodul Makefile
#
###############################################################################

# navn på kjernemodul (navnet på C-fila men uten ".c")
MODULE = buttons
ADDR = 129.241.103.222:/root

ifneq ($(KERNELRELEASE),)
obj-m := ${MODULE}.o

else

# sti til linux-kildekoden
KERNELDIR := /opt/atstk1002_bsp/build/linux-2.6.35.4

default:
	$(MAKE) -C $(KERNELDIR) M=$$PWD modules

endif

.PHONY : send
send:
	$(shell scp ${MODULE}.ko root@${ADDR})

.PHONY : clean
clean:
	rm -rf *.o *.ko .*.cmd *.mod.* .tmp*  Module.symvers modules.order Module.symvers modules.order

