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
// gpio header
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>


#undef pr_fmt
#define pr_fmt(fmt) "%s :" fmt,__func__

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Burton Huang");
MODULE_DESCRIPTION("A gpio sysfs driver");

/*Device private data structure*/
struct gpiodev_private_data
{
    char label[20];
    struct gpio_desc *desc;
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

ssize_t direction_show(struct device *dev, struct device_attribute *attr,char *buf)
{
    return 0;
}

ssize_t direction_store(struct device *dev, struct device_attribute *attr,const char *buf, size_t count)
{
    return 0;
}

ssize_t value_show(struct device *dev, struct device_attribute *attr,char *buf)
{
    return 0;
}

ssize_t value_store(struct device *dev, struct device_attribute *attr,const char *buf, size_t count)
{
    return 0;
}

ssize_t label_show(struct device *dev, struct device_attribute *attr,char *buf)
{
    return 0;
}

static DEVICE_ATTR_RW(direction);
static DEVICE_ATTR_RW(value);
static DEVICE_ATTR_RO(label);


static int gpio_sysfs_remove(struct platform_device *pdev)
{
    return 0;
}

static int gpio_sysfs_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    const char *name;
    int i = 0;
    int ret = 0;

    /*parent device node */
    struct device_node *parent = pdev->dev.of_node;
    struct device_node *child = NULL;

    struct  gpiodev_private_data *dev_data;

    for_each_available_child_of_node(parent,child)
    {
        dev_data = devm_kzalloc(dev,sizeof(*dev_data), GFP_KERNEL);
        if(!dev_data){
            dev_err(dev,"Cannot allocate memory\n");
            return -ENOMEM;
        }

        if(of_property_read_string(child,"label",&name) )
        {
            dev_warn(dev,"Missing label information\n");
            snprintf(dev_data->label,sizeof(dev_data->label),"unkngpio%d",i);
        }else{
            strcpy(dev_data->label,name);
            dev_info(dev,"GPIO label = %s\n",dev_data->label);

        }

        dev_data->desc = devm_fwnode_get_gpiod_from_child( \
        dev,"bone",&child->fwnode,GPIOD_ASIS,dev_data->label);

        if(IS_ERR( dev_data->desc )){
            ret = PTR_ERR(dev_data->desc);
            if(ret == -ENOENT)
                dev_err(dev,"No GPIO has been assigned to the requested funtion and/or index\n");
            return ret;
        }

        /* set the gpio direction to putput */
        ret = gpiod_direction_output(dev_data->desc, 0);
        if(ret){
            dev_err(dev,"gpio direction set failed \n");
            return ret;
        }

        i++;
    }

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