#
# Makefile for the virtual touchscreen driver.
#

PI4_BONE_KERN_DIR = /home/qq/workspace/source/linux_rpi-5.4.y/
KERN_DIR := $(PI4_BONE_KERN_DIR)
HOST_KERN_DIR = /lib/modules/$(shell uname -r)/build/

KDIR ?= /lib/modules/`uname -r`/build

# obj-m := virtual_touchscreen.o
obj-m := chipone_icn8318.o


default:
	$(MAKE) -C $(KDIR) M=$$PWD modules

host:
	make -C $(HOST_KERN_DIR) M=$(PWD) modules

host_chip_one:
	obj-m := chipone_icn8318.o; make -C $(HOST_KERN_DIR) M=$(PWD) modules

clean:
	rm -f *.o *.cmd *.mod.c .*.o* .*.ko* *~ *.ko Module.markers modules.order Module.symvers 
	rm -rf .tmp_versions/
