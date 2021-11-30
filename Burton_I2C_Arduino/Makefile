obj-m:= arduino.o
ARCH =arm
CROSS_COMPLIE =arm-linux-gnueabihf-
KERN_DIR = /home/qq/workspace/ldd/source/linux_bbb_5.4/
HOST_KERN_DIR = /lib/modules/$(shell uname -r)/build/

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPLIE) -C $(KERN_DIR) M=$(PWD) modules
	
clean:
	rm arduino-i2c.dtbo

make_dtbo:
	dtc -W no-unit_address_vs_reg  -@ -I dts -O dtb -o arduino-i2c.dtbo arduino-i2c.dts

copy-dtbo:
	scp arduino-i2c.dtbo debian@192.168.7.2:/home/debian/drivers

copy-drv:
	scp *.ko  debian@192.168.7.2:/home/debian/drivers

