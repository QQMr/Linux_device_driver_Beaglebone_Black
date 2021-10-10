#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/kdev_t.h>
#include<linux/uaccess.h>
#include<linux/platform_device.h>
#include<linux/slab.h>
#include<linux/mod_devicetable.h>
#include<linux/of.h>
#include<linux/of_device.h>


#undef pr_fmt
#define pr_fmt(fmt) "%s :" fmt,__func__

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Burton Huang");
MODULE_DESCRIPTION("A gpio sysfs driver");

/*Device private data structure*/
struct gpiodev_private_data
{
    char label[20];
};

/*Driver private data structure*/
struct gpiodrv_private_data
{
    int total_devices;
    struct class *class_gpio;
};

struct gpiodrv_private_data gpio_drv_data;

struct of_device_id gpio_device_match[] = 
{
    {.compatible = "org,bone-gpio-sysfs"},
    { }
};

static int gpio_sysfs_remove(struct platform_device *pdev)
{
    return 0;
}

static int gpio_sysfs_probe(struct platform_device *pdev)
{
    return 0;
}

struct platform_driver gpiosysfs_platform_driver =
{
    /* data */
    .probe  = gpio_sysfs_probe,
    .remove = gpio_sysfs_remove,
    .driver = {
            .name = "bone-gpio-syfs",
            .of_match_table = of_match_ptr(gpio_device_match)
    },
};




static int __init gpio_sysfs_init(void)
{
	int ret;


	/*2. Create device class under /sys/class */
	gpio_drv_data.class_gpio = class_create(THIS_MODULE,"bone_gpios");
    if(IS_ERR(gpio_drv_data.class_gpio))
    {
        pr_err("Class creation failed\n");
        ret = PTR_ERR(gpio_drv_data.class_gpio);
        return ret; 
    }


	/*3. Register a platform driver*/
	platform_driver_register(&gpiosysfs_platform_driver);
	pr_info("modele load success\n");

	return 0;
}

static void __exit gpio_sysfs_exit(void)
{
    class_destroy( gpio_drv_data.class_gpio );

    platform_driver_unregister(&gpiosysfs_platform_driver);

}

module_init(gpio_sysfs_init);
module_exit(gpio_sysfs_exit);