obj-m:= dummy_spi_chrdrv.o
# obj-m+= arduino_Char.o
ARCH =arm
CROSS_COMPLIE =arm-linux-gnueabihf-
BEAGLE_BONE_KERN_DIR = /home/qq/workspace/ldd/source/linux_bbb_5.4/
PI4_BONE_KERN_DIR = /home/qq/workspace/ldd/source/rpi4/linux/
KERN_DIR := $(PI4_BONE_KERN_DIR)
HOST_KERN_DIR = /lib/modules/$(shell uname -r)/build/
ccflags-y += -std=gnu99
ccflags-y += -Wno-declaration-after-statement
empty = 1
bar := $(if $(empty),then!,else!)
LOCAL_IP := $(if $(PI4_IP),$(PI4_IP),192.168.46.175)

all:
	echo $(ccflags-y)
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPLIE) -C $(KERN_DIR) M=$(PWD) modules
	
clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPLIE) -C $(KERN_DIR) M=$(PWD) clean
	rm -f arduino-spi.dtbo
	rm -f *.out

host:
	make -C $(HOST_KERN_DIR) M=$(PWD) modules

make_dtbo2:
	$(KERN_DIR)/scripts/dtc/dtc -I dts -O dtb -o arduino-spi.dtbo arduino-spi.dts

copy-dtbo:
	@echo "Use PI4 setting"
	scp arduino-spi.dtbo qq@${LOCAL_IP}:/home/qq/driver

copy-drv:
	scp *.ko  qq@${LOCAL_IP}:/home/qq/driver

copy-out:
	scp *.out  debian@${LOCAL_IP}:/home/debian/drivers

read-drv1:
	# ssh -t qq@${LOCAL_IP} sudo od -N 32 -a /dev/arduino
	ssh -t qq@${LOCAL_IP} "dd if=/dev/arduino bs=31 count=1 | hexdump -C"

write-drv:
	ssh -t qq@${LOCAL_IP} "sudo echo -n -e '\x5A\x01\x10' > /dev/arduino"

