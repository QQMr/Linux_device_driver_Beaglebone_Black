
PI4_BONE_KERN_DIR = /home/qq/workspace/source/linux_rpi-5.4.y_test/
KERN_DIR := $(PI4_BONE_KERN_DIR)
HOST_KERN_DIR = /lib/modules/$(shell uname -r)/build/
ccflags-y += -std=gnu99
ccflags-y += -Wno-declaration-after-statement
empty = 1
bar := $(if $(empty),then!,else!)

replace-pinctrl-bcm2835:
	@echo "Use PI4 setting"
	@echo ${KERN_DIR}/drivers/pinctrl/bcm/pinctrl-bcm28355.c
#	cp -f pinctrl-bcm2835.c ${KERN_DIR}/drivers/pinctrl/bcm/pinctrl-bcm28355.c


make-kernel_img:
#	cp -f *E007* ~/workspace/ldd/source/linux_bbb_5.4/arch/arm/boot/dts/
	cd ${KERN_DIR}; pwd;\
	pwd;
#	cd ~/workspace/ldd/source/linux_bbb_5.4; make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- am335x-boneblack-wireless-E007.dtb
#	cp $(burton_dts_path)/am335x-boneblack-wireless-E007.dtb ./


copy-kernel_img:


