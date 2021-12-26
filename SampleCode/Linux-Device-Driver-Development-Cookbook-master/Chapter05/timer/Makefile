ARCH =arm
CROSS_COMPLIE =arm-linux-gnueabihf-
KERN_DIR = /home/qq/workspace/ldd/source/linux_bbb_5.4/
HOST_KERN_DIR = /lib/modules/$(shell uname -r)/build/
ccflags-y += -std=gnu99
ccflags-y += -Wno-declaration-after-statement


obj-m += hires_timer.o
obj-m += ktimer.o

# The default action
all:
	echo $(ccflags-y)
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPLIE) -C $(KERN_DIR) M=$(PWD) modules

clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPLIE) -C $(KERN_DIR) M=$(PWD) clean

host:
	make -C $(HOST_KERN_DIR) M=$(PWD) modules

copy-drv:
	scp *.ko  debian@192.168.7.2:/home/debian/drivers
