#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spi/spi.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define ARDUINO_DEV_NAME "arduino"
static struct class *arduino_class = NULL;
static dev_t dev = 0;

struct arduino_spi_cdev {
    struct spi_device *spi;
    struct cdev cdev;
};

static ssize_t arduino_spi_open(struct inode *inode, struct file *filp)
{
    struct arduino_spi_cdev *arduino = container_of(inode->i_cdev, struct arduino_spi_cdev, cdev);
    if (!arduino) {
    	pr_err("Cannot extrace aduino structure from i_cdev.\n");
	return -EINVAL;
    }
    filp -> private_data = arduino -> spi;
    return 0;
}

static ssize_t arduino_spi_write(struct file *filp, const char __user *buf, size_t count,
		loff_t *offset)
{
    int err = 0;
    struct spi_device *spi = filp -> private_data;
    if (!spi) {
    	pr_err("Failed to get struct spi_device.\n");
	return -EINVAL;
    }
    char *msg = kzalloc(count + 1, GFP_KERNEL);
    copy_from_user(msg, buf, count);
    err = spi_write(spi, msg, count);
    kfree(msg);
    return err ? err : count;
}


#if 0
static ssize_t arduino_spi_read(struct file * filp, char __user *buff, size_t count, loff_t *f_pos)
{

    //   struct i2c_client *client = filp -> private_data;
	// struct pcdev_private_data *pcdev_data = (struct pcdev_private_data*)filp->private_data;

	//int max_size = pcdev_data->size;
	//char *device_buffer = pcdev_data->buffer;

    char tBuffer[32];
    int max_size = 32;
	char *device_buffer = tBuffer;

	pr_info("read requested for %zu bytes \n",count);
	pr_info("current file position = %lld",*f_pos);

    for(int i=0;i<32;i++)
    {
        tBuffer[i] = '0'+i;
    }


	/* Adjust the 'count'  */
	if( (*f_pos+count) > max_size  )
		count = max_size - *f_pos;

    // i2c_smbus_read_i2c_block_data(client, 0x30, count, device_buffer);

	/* copy to user*/
	if( copy_to_user(buff,&device_buffer[*f_pos],count) ){
		return -EFAULT;
	}

	/*update the current file position */
	*f_pos += count;

	pr_info("Number of bytes succcesfully read = %zu\n",count);
	pr_info("Update file position = %lld\n",*f_pos);

	/*Return number of bytes which have been succssfully read*/

	return count;

}
#else
static ssize_t arduino_spi_read(struct file * filp, char __user *buff, size_t count, loff_t *f_pos)
{
    int err = 0;
    struct spi_device *spi = filp -> private_data;
    if (!spi) {
    	pr_err("Failed to get struct spi_device.\n");
	return -EINVAL;
    }
    char *msg = kzalloc(count + 1, GFP_KERNEL);
    err = spi_read(spi, msg, count);
    
    /* copy to user*/
	if( copy_to_user(buff, msg,count) ){
		return -EFAULT;
	}

    kfree(msg);

    /*update the current file position */
	*f_pos += count;
    *f_pos = 0;

    return err ? err : count;
}

// {

//     //   struct i2c_client *client = filp -> private_data;
// 	// struct pcdev_private_data *pcdev_data = (struct pcdev_private_data*)filp->private_data;

// 	//int max_size = pcdev_data->size;
// 	//char *device_buffer = pcdev_data->buffer;

//     char tBuffer[32];
//     int max_size = 32;
// 	char *device_buffer = tBuffer;

// 	pr_info("read requested for %zu bytes \n",count);
// 	pr_info("current file position = %lld",*f_pos);

//     for(int i=0;i<32;i++)
//     {
//         tBuffer[i] = '0'+i;
//     }

// spi_read

// 	/* Adjust the 'count'  */
// 	if( (*f_pos+count) > max_size  )
// 		count = max_size - *f_pos;

//     // i2c_smbus_read_i2c_block_data(client, 0x30, count, device_buffer);

// 	/* copy to user*/
// 	if( copy_to_user(buff,&device_buffer[*f_pos],count) ){
// 		return -EFAULT;
// 	}

// 	/*update the current file position */
// 	*f_pos += count;

// 	pr_info("Number of bytes succcesfully read = %zu\n",count);
// 	pr_info("Update file position = %lld\n",*f_pos);

// 	/*Return number of bytes which have been succssfully read*/

// 	return count;

// }
#endif

struct file_operations arduino_spi_fops = {
    .open = arduino_spi_open,
    .write = arduino_spi_write,
    .read = arduino_spi_read,
};


static int dummy_probe(struct spi_device *spi)
{
    int err = 0;
    pr_info("Dummy device is being probed.\n");

    err = alloc_chrdev_region(&dev, 0, 1, ARDUINO_DEV_NAME);
    if (err < 0) {
        pr_err ("Failed in alloc_chrdev_reion for arduino.\n");
	goto out_alloc_chrdev;
    }

    //arduino_class = class_create(THIS_MODULE, ARDUINO_DEV_NAME);
    arduino_class = class_create(ARDUINO_DEV_NAME);
    if (!arduino_class) {
    	pr_err ("Failed to create sysfs class.\n");
	goto out_sysfs_class;
    }

    struct arduino_spi_cdev *arduino = kzalloc(sizeof(struct arduino_spi_cdev), GFP_KERNEL);
    if (!arduino) {
	pr_err("Failed to allocate memory.\n");
    	goto out_oom;
    }
    arduino -> spi = spi;

    cdev_init(&(arduino -> cdev), &arduino_spi_fops);
    arduino->cdev.owner = THIS_MODULE;
    err = cdev_add(&(arduino -> cdev), dev, 1);
    if (err) {
	pr_err("Failed to register cdev.\n");
    	goto out_cdev_add;
    }

    struct device *device = device_create(arduino_class, NULL, dev, NULL, ARDUINO_DEV_NAME);
    if (!device) {
    	pr_err("Failed to create device entry under sysfs.\n");
	goto out_device;
    }
    spi->max_speed_hz = 400000;
    dev_set_drvdata(&(spi->dev), arduino);
    return 0;

out_device:
    cdev_del(&arduino->cdev);
out_cdev_add:
    kfree(arduino);
out_oom:
    class_destroy(arduino_class);
out_sysfs_class:
    unregister_chrdev_region(dev, 1);
out_alloc_chrdev:
    return err;    
}

static void dummy_remove(struct spi_device *spi)
{
    pr_info("Dummy device is removing.\n");
    struct arduino_spi_cdev *arduino = dev_get_drvdata(&(spi->dev));
    device_destroy(arduino_class, dev);
    cdev_del(&(arduino->cdev));
    kfree(arduino);
    class_destroy(arduino_class);
    unregister_chrdev_region(dev, 1);
    return;
}

static struct of_device_id dummy_id_tables [] = {
    { .compatible="arduino", },
    { }
};
MODULE_DEVICE_TABLE(of, dummy_id_tables);

static struct spi_driver dummy_drv = {
    .probe = dummy_probe,
    .remove = dummy_remove,
    .driver = {
    	.name = "dummy device 0.1",
	.owner = THIS_MODULE,
	.of_match_table = dummy_id_tables,
    },
};

module_spi_driver(dummy_drv);
MODULE_LICENSE("GPL");