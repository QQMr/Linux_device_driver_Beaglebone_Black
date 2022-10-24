
PI4_BONE_KERN_DIR = /home/qq/workspace/source/linux_rpi-5.4.y_test/
KERN_DIR := $(PI4_BONE_KERN_DIR)

LOCAL_IP := $(if $(PI4_IP),$(PI4_IP),192.168.46.175)
empty = 1
bar := $(if $(empty),then!,else!)

replace-pinctrl-bcm2835:
	@echo "Use PI4 setting"
	@echo ${KERN_DIR}drivers/pinctrl/bcm/pinctrl-bcm28355.c
	cp -f pinctrl-bcm2835.c ${KERN_DIR}/drivers/pinctrl/bcm/pinctrl-bcm2835.c


make-kernel_img: replace-pinctrl-bcm2835
#	cp -f *E007* ~/workspace/ldd/source/linux_bbb_5.4/arch/arm/boot/dts/
#	cd ${KERN_DIR}; pwd;\
	pwd;
	KERNEL=kernel7l; echo $$KERNEL; \
	$(MAKE) -C ${KERN_DIR} -j4 ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage;
#	cd ~/workspace/ldd/source/linux_bbb_5.4; make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- am335x-boneblack-wireless-E007.dtb
#	cp $(burton_dts_path)/am335x-boneblack-wireless-E007.dtb ./


copy-kernel_img:
#	scp lcd-hd44780-i2c.dtbo qq@192.168.7.2:/home/debian/drivers
	echo "IP is" ${LOCAL_IP}
#	KERNEL=kernel7l; scp ${KERN_DIR}/arch/arm/boot/zImage qq@${LOCAL_IP}:/boot/$$KERNEL"_new.img"
	KERNEL=kernel7l; echo ${KERN_DIR}/arch/arm/boot/zImage qq@${LOCAL_IP}:/boot/$$KERNEL"_new.img"
	KERNEL=kernel7l; scp ${KERN_DIR}/arch/arm/boot/zImage qq@${LOCAL_IP}:~/$$KERNEL"_new.img"


