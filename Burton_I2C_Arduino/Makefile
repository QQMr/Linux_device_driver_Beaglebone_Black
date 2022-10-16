obj-m:= arduino.o
obj-m+= arduino_Char.o
ARCH =arm
CROSS_COMPLIE =arm-linux-gnueabihf-
# KERN_DIR = /home/qq/workspace/ldd/source/linux_bbb_5.4/
BEAGLE_BONE_KERN_DIR = /home/qq/workspace/ldd/source/linux_bbb_5.4/
PI4_BONE_KERN_DIR = /home/qq/workspace/source/linux_rpi-5.4.y/
KERN_DIR := $(if $(PI4),$(PI4_BONE_KERN_DIR),$(BEAGLE_BONE_KERN_DIR))
HOST_KERN_DIR = /lib/modules/$(shell uname -r)/build/
ccflags-y += -std=gnu99
ccflags-y += -Wno-declaration-after-statement
empty = 1
bar := $(if $(empty),then!,else!)

all:
	echo $(ccflags-y)
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPLIE) -C $(KERN_DIR) M=$(PWD) modules
	
clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPLIE) -C $(KERN_DIR) M=$(PWD) clean
	rm -f arduino-i2c.dtbo
	rm -f *.out

host:
	make -C $(HOST_KERN_DIR) M=$(PWD) modules
test_Char:
	$(CROSS_COMPLIE)gcc -o test_arduinoChar.out test_arduinoChar.c

make_dtbo:
ifdef PI4
	@echo "Use PI4 setting"
	dtc -W no-unit_address_vs_reg  -@ -I dts -O dtb -o arduino-i2c-pi4.dtbo arduino-i2c-pi4.dts
endif
ifndef PI4
	dtc -W no-unit_address_vs_reg  -@ -I dts -O dtb -o arduino-i2c.dtbo arduino-i2c.dts
endif

copy-dtbo:
ifdef PI4
	@echo "Use PI4 setting"
	scp arduino-i2c-pi4.dtbo qq@192.168.113.175:/home/qq/driver
endif
ifndef PI4
	scp arduino-i2c.dtbo debian@192.168.7.2:/home/debian/drivers
endif

copy-drv:
ifdef PI4
	@echo "Use PI4 setting"
	scp *.ko  qq@192.168.113.175:/home/qq/driver
endif
ifndef PI4
	scp *.ko  debian@192.168.7.2:/home/debian/drivers
endif

copy-out:
	scp *.out  debian@192.168.7.2:/home/debian/drivers

