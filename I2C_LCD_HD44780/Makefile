obj-m:= LcdHD44780.o
LcdHD44780-objs += ./Arduino_HD44780Driver/HD44780U_I2C_Burton.o LCD_DelayWork.o I2c_LcdHD44780.o
ARCH =arm
CROSS_COMPLIE =arm-linux-gnueabihf-
KERN_DIR = /home/qq/workspace/ldd/source/linux_bbb_5.4/
HOST_KERN_DIR = /lib/modules/$(shell uname -r)/build/
ccflags-y += -std=gnu99
ccflags-y += -Wno-declaration-after-statement

all:
	echo $(ccflags-y)
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPLIE) -C $(KERN_DIR) M=$(PWD) modules
	
clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPLIE) -C $(KERN_DIR) M=$(PWD) clean
	rm -f lcd-hd44780-i2c.dtbo

host:
	make -C $(HOST_KERN_DIR) M=$(PWD) modules

make_dtbo:
	dtc -W no-unit_address_vs_reg  -@ -I dts -O dtb -o lcd-hd44780-i2c.dtbo lcd-hd44780-i2c.dts

copy-dtbo:
	scp lcd-hd44780-i2c.dtbo debian@192.168.7.2:/home/debian/drivers

copy-drv:
	scp *.ko  debian@192.168.7.2:/home/debian/drivers

copy-out:
	scp *.out  debian@192.168.7.2:/home/debian/drivers

