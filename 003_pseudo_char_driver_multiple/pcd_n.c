#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/kdev_t.h>
#include<linux/uaccess.h>

#undef pr_fmt
#define pr_fmt(fmt) "%s :" fmt,__func__

#define NO_OF_DEVICES 4

#define MEM_SIZE_MAX_PCDEV1 1024
#define MEM_SIZE_MAX_PCDEV2 512
#define MEM_SIZE_MAX_PCDEV3 1024
#define MEM_SIZE_MAX_PCDEV4 512

/* pseudo device's memory */
char device_buffer_pcdev1[MEM_SIZE_MAX_PCDEV1];
char device_buffer_pcdev2[MEM_SIZE_MAX_PCDEV2];
char device_buffer_pcdev3[MEM_SIZE_MAX_PCDEV3];
char device_buffer_pcdev4[MEM_SIZE_MAX_PCDEV4];


/*Device private data structure*/

struct pcdev_private_data
{
	char *buffer;
	unsigned size;
	const char *serial_number;
	int perm;
	/*Cdev variable*/
	struct cdev cdev;
};

/*Driver private data structure */
struct pcdrv_private_data
{
	int total_devices;
	/* this holds the device number*/
	dev_t device_number;
	struct class *class_pcd;
	struct device *device_pcd;
	struct pcdev_private_data pcdev_data[NO_OF_DEVICES];
};

struct pcdrv_private_data pcdrv_data =
{
	.total_devices = NO_OF_DEVICES,
	.pcdev_data = {

		[0]= {
			.buffer = device_buffer_pcdev1,
			.size = MEM_SIZE_MAX_PCDEV1,
			.serial_number = "PCDEV1XYZ123",
			.perm = 0x01 /*RDONLY*/
		},
                [1]= {
                        .buffer = device_buffer_pcdev2,
                        .size = MEM_SIZE_MAX_PCDEV2,
                        .serial_number = "PCDEV2XYZ123",
                        .perm = 0x10 /*RDONLY*/
                },
                [2]= {
                        .buffer = device_buffer_pcdev3,
                        .size = MEM_SIZE_MAX_PCDEV3,
                        .serial_number = "PCDEV1XYZ123",
                        .perm = 0x11 /*RDONLY*/
                },
                [3]= {
                        .buffer = device_buffer_pcdev4,
                        .size = MEM_SIZE_MAX_PCDEV4,
                        .serial_number = "PCDEV4XYZ123",
                        .perm = 0x11 /*RDONLY*/
                }
	}
};


loff_t pcd_lseek(struct file *filp, loff_t offset, int whence)
{
#if 0
	loff_t temp;
	pr_info("lseek requested");
	pr_info("Current value of the file position =%lld\n",filp->f_pos);

	switch(whence)
	{
		case SEEK_SET:
			if( (offset > DEV_MEM_SIZE) || offset < 0 )
				return -EINVAL; 
			filp->f_pos = offset;
			break;
		case SEEK_CUR:
			temp = filp->f_pos + offset;
			if((temp > DEV_MEM_SIZE)|| temp < 0 )
				return -EINVAL; 
			filp->f_pos = temp;
			break;
		case SEEK_END:
			temp = DEV_MEM_SIZE + offset;
                        if((temp > DEV_MEM_SIZE)|| temp < 0 )
                                return -EINVAL;
			filp->f_pos = temp;
			break;
		default:
			return -EINVAL;

	
	}

	 pr_info("New value of the file position =%lld\n",filp->f_pos);
	return DEV_MEM_SIZE;
#endif
	return 0;
}

ssize_t pcd_read(struct file * filp, char __user *buff, size_t count, loff_t *f_pos)
{
#if 0
	pr_info("read requested for %zu bytes \n",count);
	pr_info("current file position = %lld",*f_pos);

	/* Adjust the 'count'  */
	if( (*f_pos+count) > DEV_MEM_SIZE  )
		count = DEV_MEM_SIZE - *f_pos;

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
#endif
	return 0;
}

ssize_t pcd_write(struct file * filp, const char __user *buff, size_t count, loff_t *f_pos)
{
#if 0
	pr_info("write requested for %zu bytes \n",count);
	pr_info("current file position = %lld",*f_pos);

        /* Adjust the 'count'  */
        if( (*f_pos+count) > DEV_MEM_SIZE  )
                count = DEV_MEM_SIZE - *f_pos;

	if(!count){
		pr_err("No space left on the device\n");
		return -ENOMEM;
	}

        /* copy from user*/
        if( copy_from_user(&device_buffer[*f_pos],buff,count) ){
                return -EFAULT;
        }

        /*update the current file position */
        *f_pos += count;

        pr_info("Number of bytes succcesfully written  = %zu\n",count);
        pr_info("Update file position = %lld\n",*f_pos);

        /*Return number of bytes which have been succssfully written*/
	return count;
#endif
	return -ENOMEM;
}

int pcd_open (struct inode *inode, struct file *filp)
{
	pr_info("open was successful\n");
	return 0;
}

int pcd_release(struct inode *inode, struct file *filp)
{
	pr_info("release was successful\n");
	return 0;
}

/* file operations pcd_fops;*/
struct file_operations pcd_fops=
{
	.open = pcd_open,
	.write = pcd_write,
	.read = pcd_read,
	.llseek = pcd_lseek,
	.release = pcd_release,
	.owner = THIS_MODULE
};


static int __init pcd_driver_init(void)
{
#if 0
	int ret;

	/*1. Dynamically allocate a device number*/
	ret = alloc_chrdev_region(&device_number,0,1,"pcd_devices");
	if(ret < 0){
		pr_err("Alloc chrdev failed");
		goto out;
	}

	pr_info("Device number <major>/<minor> = %d %d\n",MAJOR(device_number),MINOR(device_number));

	/*2. Initialize the cdev structure with fops*/
	cdev_init(&pcd_cdev,&pcd_fops);

	/*3. Register a device(cdev structure) with VFS */
	pcd_cdev.owner = THIS_MODULE;
	ret = cdev_add(&pcd_cdev,device_number,1);
	if(ret < 0){
		pr_err("Cdev add failed");
		goto unreg_chrdev;
	}

	/*4. create device class under /sys/class/ */
	class_pcd = class_create(THIS_MODULE,"pcd_class");
	if(IS_ERR(class_pcd))
	{
		pr_err("Class creation failed\n");
		ret = PTR_ERR(class_pcd);
		goto cdev_del;
	}

	/*5. populate the sysfs with device information */
	device_pcd = device_create(class_pcd,NULL,device_number,NULL,"pcd");
        if(IS_ERR(device_pcd))
        {
                pr_err("Device creation failed\n");
                ret = PTR_ERR(device_pcd);
                goto class_del;
        }

	pr_info("Module init was successful\n");

	return 0;

class_del:
	class_destroy(class_pcd);
cdev_del:
	cdev_del(&pcd_cdev);
unreg_chrdev:
	unregister_chrdev_region(device_number,1);
out:
	pr_info("Module insertion failed\n");
	return ret;
#endif
	return 0;
}



static void __exit pcd_driver_exit(void)
{
#if 0
	device_destroy(class_pcd,device_number);
	class_destroy(class_pcd);
	cdev_del(&pcd_cdev);
	unregister_chrdev_region(device_number,1);
	pr_info("module unloaded\n");
#endif
	return;
}

module_init(pcd_driver_init);
module_exit(pcd_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ME");
MODULE_DESCRIPTION("A pseudo character driver which handles n devices");
MODULE_INFO(board,"beagle bone wireless");
