obj-m:= main.o
ARCH =arm
CROSS_COMPLIE =arm-linux-gnueabihf-
BEAGLE_BONE_KERN_DIR = /home/qq/workspace/ldd/source/linux_bbb_5.4/
PI4_BONE_KERN_DIR = /home/qq/workspace/source/linux_rpi-5.4.y/
KERN_DIR := $(if $(PI4),$(PI4_BONE_KERN_DIR),$(BEAGLE_BONE_KERN_DIR))
HOST_KERN_DIR = /lib/modules/$(shell uname -r)/build/

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPLIE) -C $(KERN_DIR) M=$(PWD) modules
clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPLIE) -C $(KERN_DIR) M=$(PWD) clean
help:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPLIE) -C $(KERN_DIR) M=$(PWD) help
host:
	make -C $(HOST_KERN_DIR) M=$(PWD) modules

copy-drv:
ifdef PI4
	@echo "Use PI4 setting"
	scp *.ko  qq@192.168.113.175:/home/qq/driver
endif
ifndef PI4
	scp *.ko  debian@192.168.7.2:/home/debian/drivers
endif
