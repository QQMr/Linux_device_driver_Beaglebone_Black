obj-m:= int_rpi4_key.o
# obj-m+= arduino_Char.o
ARCH =arm
CROSS_COMPLIE =arm-linux-gnueabihf-
BEAGLE_BONE_KERN_DIR = /home/qq/workspace/ldd/source/linux_bbb_5.4/
PI4_BONE_KERN_DIR = /home/qq/workspace/source/linux_rpi-5.4.y/
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
	rm -f int_rpi4_key.dtbo
	rm -f *.out

host:
	make -C $(HOST_KERN_DIR) M=$(PWD) modules

make_dtbo:
# dtc -@ -I dts -O dtb -o arduino-irq.dtbo arduino-irq.dts
# dtc -W no-unit_address_vs_reg  -@ -I dts -O dtb -o arduino-irq.dtbo arduino-irq.dts
# cp -f int_rpi4_key.dts /home/qq/workspace/source/linux_rpi-5.4.y/arch/arm/boot/dts/
# cd /home/qq/workspace/source/linux_rpi-5.4.y/; pwd
# cd /home/qq/workspace/source/linux_rpi-5.4.y/; make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- int_rpi4_key.dtbo
# cp /home/qq/workspace/source/linux_rpi-5.4.y/int_rpi4_key.dtbo ./
	cp -f int_rpi4_key.dts $(KERN_DIR)/arch/arm/boot/dts/overlays/int_rpi4_key-overlay.dts
	cd $(KERN_DIR); make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPLIE) dtbs
	cp -f $(KERN_DIR)/arch/arm/boot/dts/overlays/int_rpi4_key.dtbo ./

copy-dtbo:
	@echo "Use PI4 setting"
	scp int_rpi4_key.dtbo qq@${LOCAL_IP}:/home/qq/driver

copy-drv:
	scp *.ko  qq@${LOCAL_IP}:/home/qq/driver

copy-out:
	scp *.out  debian@${LOCAL_IP}:/home/debian/drivers

