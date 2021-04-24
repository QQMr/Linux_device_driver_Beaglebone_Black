#include<linux/module.h>
#include<linux/platform_device.h>


#include "platform.h"

#undef pr_fmt
#define pr_fmt(fmt) "%s :" fmt,__func__

void pcdev_release(struct device *dev)
{
	pr_info("Device release\n ");
}


//1: create 2 platform data

struct pcdev_platform_data pcdev_data[2]= {
	[0] = {.size=512, .perm = RDWR, .serial_number = "PCDEVAB1111"},
	[1] = {.size=512, .perm = RDWR, .serial_number = "PCDEVAB2222"},
};


//2: create 2 platform device


struct platform_device platform_pcdev_1 = {
	.name = "pseudo-char-device",
	.id = 0,
	.dev = {
		.platform_data = &pcdev_data[0],
		.release = pcdev_release
	}
};

struct platform_device platform_pcdev_2 = {
        .name = "pseudo-char-device",
        .id = 1,
	.dev = {
                .platform_data = &pcdev_data[1],
		.release = pcdev_release
        }

};



static int __init pcdev_platform_init(void)
{
	//register platform
	platform_device_register(&platform_pcdev_1);
	platform_device_register(&platform_pcdev_2);

	pr_info("Device setup module inserted\n");

	return 0;
}


static void __exit pcdev_platform_exit(void)
{

	platform_device_unregister(&platform_pcdev_1);
	platform_device_unregister(&platform_pcdev_2);

	pr_info("Device setup module remove\n");
}

module_init(pcdev_platform_init);
module_exit(pcdev_platform_exit);


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module which registers platform devices ");
