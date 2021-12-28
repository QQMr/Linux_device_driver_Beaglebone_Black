#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/uaccess.h>
# include "./Arduino_HD44780Driver/HD44780U_I2C_Burton.h"

#define ARDUINO_DEV_NAME "LCD_HD44780"
static struct class *arduino_class = NULL;
static dev_t dev = 0;

struct arduino_i2c_cdev {
    struct i2c_client *client;
    struct cdev cdev;
};

static int arduino_i2c_open(struct inode *inode, struct file *filp)
{
    // struct arduino_i2c_cdev *arduino = container_of(inode->i_cdev, struct arduino_i2c_cdev, cdev);
    // pr_info("Dummy device OPEN.\n");
    // if (!arduino) {
    // 	pr_err("Cannot extrace aduino structure from i_cdev.\n");
	// return -EINVAL;
    // }
    // filp -> private_data = arduino -> client;
    pr_info("Dummy device OPEN-END.\n");
    return 0;
}

static ssize_t arduino_i2c_write(struct file *filp, const char __user *buf, size_t count,
		loff_t *offset)
{
    int i =0;
    char device_buffer[1000];
    // struct i2c_client *client = filp -> private_data;
    pr_info("Dummy device WRITE.\n");
    // if (!client) {
    // 	pr_err("Failed to get struct i2c_client.\n");
	//     return -EINVAL;
    // }

    // if( count > 1000 ){
    // 	pr_err("write count is too much\n");
	//     return -EINVAL;
    // }

    if( count > 41 ){
    	pr_err("write count is too much\n");
	    return -EINVAL;
    }

    if( copy_from_user(&device_buffer,buf,count) ){
            return -EFAULT;
    }
    if(device_buffer[count-1] !='\0')
    {
        device_buffer[count-1] = '\0';
    }
    pr_info("Dummy device WRITE-2.\n");
    // for (int i = 0; i < count; i++) {
    //     i2c_smbus_write_byte(client, device_buffer[i]);
    // }
    clear();
    WriteMessage(device_buffer);
    for(i=count-1;i<40;i++)
    {
        WriteMessage(" ");
    }
    //WriteMessage("AAAAAAAAAAAA");
    WriteMessage(device_buffer);
    pr_info("Dummy device WRITE-END.\n");
    return count;
}


ssize_t arduino_i2c_read(struct file * filp, char __user *buff, size_t count, loff_t *f_pos)
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

struct file_operations arduino_i2c_fops = {
    .open = arduino_i2c_open,
    .write = arduino_i2c_write,
    .read = arduino_i2c_read,
};

static struct arduino_i2c_cdev *myArduino;
int char_probe(void)
{

    int err = 0;

    err = alloc_chrdev_region(&dev, 0, 1, ARDUINO_DEV_NAME);
    if (err < 0) {
        pr_err ("Failed in alloc_chrdev_reion for arduino.\n");
	goto out_alloc_chrdev;
    }

    arduino_class = class_create(THIS_MODULE, ARDUINO_DEV_NAME);
    if (!arduino_class) {
    	pr_err ("Failed to create sysfs class.\n");
	goto out_sysfs_class;
    }

    myArduino = kzalloc(sizeof(struct arduino_i2c_cdev), GFP_KERNEL);
    if (!myArduino) {
	pr_err("Failed to allocate memory.\n");
    	goto out_oom;
    }
    // arduino -> client = client;

    cdev_init(&(myArduino -> cdev), &arduino_i2c_fops);
    myArduino->cdev.owner = THIS_MODULE;
    err = cdev_add(&(myArduino -> cdev), dev, 1);
    if (err) {
	pr_err("Failed to register cdev.\n");
    	goto out_cdev_add;
    }

    struct device *device = device_create(arduino_class, NULL, dev, NULL, ARDUINO_DEV_NAME);
    if (!device) {
    	pr_err("Failed to create device entry under sysfs.\n");
	goto out_device;
    }
    // i2c_set_clientdata(client, arduino);
    return 0;

out_device:
    cdev_del(&myArduino->cdev);
out_cdev_add:
    kfree(myArduino);
out_oom:
    class_destroy(arduino_class);
out_sysfs_class:
    unregister_chrdev_region(dev, 1);
out_alloc_chrdev:
    return err;    
}

int char_remove(void)
{
    pr_info("Dummy device is removing.\n");
    // struct arduino_i2c_cdev *arduino = i2c_get_clientdata(client);
    struct arduino_i2c_cdev *arduino = myArduino;
    device_destroy(arduino_class, dev);
    cdev_del(&(arduino->cdev));
    kfree(arduino);
    class_destroy(arduino_class);
    unregister_chrdev_region(dev, 1);
    return 0;
}

// static struct of_device_id dummy_id_tables [] = {
//     { .compatible="arduino", },
//     { }
// };

// MODULE_DEVICE_TABLE(of, dummy_id_tables);

// static struct i2c_driver dummy_drv = {
//     .probe = dummy_probe,
//     .remove = dummy_remove,
//     .driver = {
//     	.name = "dummy device 0.1",
// 	    .owner = THIS_MODULE,
// 	    .of_match_table = dummy_id_tables,
//     },
// };

// module_i2c_driver(dummy_drv);
MODULE_LICENSE("GPL");