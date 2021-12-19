obj-m:= gpio_sysfs.o
gpio_sysfs-objs += gpio-sysfs.o
ARCH =arm
CROSS_COMPLIE =arm-linux-gnueabihf-
KERN_DIR = /home/qq/workspace/ldd/source/linux_bbb_5.4/
HOST_KERN_DIR = /lib/modules/$(shell uname -r)/build/
burton_dts_path=~/workspace/ldd/source/linux_bbb_5.4/arch/arm/boot/dts

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPLIE) -C $(KERN_DIR) M=$(PWD) modules
clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPLIE) -C $(KERN_DIR) M=$(PWD) clean
help:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPLIE) -C $(KERN_DIR) M=$(PWD) help
host:
	make -C $(HOST_KERN_DIR) M=$(PWD) modules

make_dtb: 
	cp -f *E008* ~/workspace/ldd/source/linux_bbb_5.4/arch/arm/boot/dts/
	cd ~/workspace/ldd/source/linux_bbb_5.4; pwd
	cd ~/workspace/ldd/source/linux_bbb_5.4; make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- am335x-boneblack-wireless-E008.dtb
	cp $(burton_dts_path)/am335x-boneblack-wireless-E008.dtb ./

copy-dtb:
	scp /home/qq/workspace/ldd/source/linux_bbb_5.4/arch/arm/boot/dts/am335x-boneblack-wireless-E008.dtb debian@192.168.7.2:/home/debian/drivers

copy-drv:
	scp *.ko  debian@192.168.7.2:/home/debian/drivers



