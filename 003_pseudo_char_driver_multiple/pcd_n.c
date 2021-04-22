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

	
        struct pcdev_private_data *pcdev_data = (struct pcdev_private_data*)filp->private_data;

        int max_size = pcdev_data->size;


	loff_t temp;
	pr_info("lseek requested");
	pr_info("Current value of the file position =%lld\n",filp->f_pos);

	switch(whence)
	{
		case SEEK_SET:
			if( (offset > max_size) || offset < 0 )
				return -EINVAL; 
			filp->f_pos = offset;
			break;
		case SEEK_CUR:
			temp = filp->f_pos + offset;
			if((temp > max_size)|| temp < 0 )
				return -EINVAL; 
			filp->f_pos = temp;
			break;
		case SEEK_END:
			temp = max_size + offset;
                        if((temp > max_size)|| temp < 0 )
                                return -EINVAL;
			filp->f_pos = temp;
			break;
		default:
			return -EINVAL;

	
	}

	 pr_info("New value of the file position =%lld\n",filp->f_pos);
	return filp->f_pos;
}

ssize_t pcd_read(struct file * filp, char __user *buff, size_t count, loff_t *f_pos)
{

	struct pcdev_private_data *pcdev_data = (struct pcdev_private_data*)filp->private_data;

	int max_size = pcdev_data->size;
	char *device_buffer = pcdev_data->buffer;

	pr_info("read requested for %zu bytes \n",count);
	pr_info("current file position = %lld",*f_pos);

	/* Adjust the 'count'  */
	if( (*f_pos+count) > max_size  )
		count = max_size - *f_pos;

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

ssize_t pcd_write(struct file * filp, const char __user *buff, size_t count, loff_t *f_pos)
{
	
	struct pcdev_private_data *pcdev_data = (struct pcdev_private_data*)filp->private_data;

        int max_size = pcdev_data->size;
        char *device_buffer = pcdev_data->buffer;


	pr_info("write requested for %zu bytes \n",count);
	pr_info("current file position = %lld",*f_pos);

        /* Adjust the 'count'  */
        if( (*f_pos+count) > max_size  )
                count = max_size - *f_pos;

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
}

int check_permission(void)
{
	return 0;
}

int pcd_open (struct inode *inode, struct file *filp)
{
	int ret;
	int minor_n;
	struct pcdev_private_data *pcdev_data;
	/*find out on which device file open was attempted by the user space*/

	minor_n = MINOR(inode->i_rdev);
	pr_info("minor access = %d\n",minor_n);

	/*get device's private data structure */
	pcdev_data = (struct pcdev_private_data*) container_of( inode->i_cdev,struct pcdev_private_data,cdev);

	/*to supply device private data to other methods of the driver */
	filp->private_data = pcdev_data;

	/*check permission*/
	ret = check_permission();

	(!ret)? pr_info("open was successful\n") : pr_info("open was unsuccessful\n");
	return ret;
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
	int ret;
	int i;

	/*1. Dynamically allocate a device number*/
	ret = alloc_chrdev_region(&pcdrv_data.device_number,0,NO_OF_DEVICES,"pcdevs");
	if(ret < 0){
		pr_err("Alloc chrdev failed");
		goto out;
	}


	/*create device class under /sys/class/ */
        pcdrv_data.class_pcd = class_create(THIS_MODULE,"pcd_class");
        if(IS_ERR(pcdrv_data.class_pcd))
        {
                pr_err("Class creation failed\n");
                ret = PTR_ERR(pcdrv_data.class_pcd);
                goto unreg_chrdev;
        }

	for(i=0;i<NO_OF_DEVICES;i++)
	{
		struct cdev *pcdev;
		dev_t device_number;
		pr_info("Device number <major>/<minor> = %d %d\n",MAJOR(pcdrv_data.device_number+i),MINOR(pcdrv_data.device_number+i));
		device_number = pcdrv_data.device_number+i;
		
		/*Initialize the cdev structure with fops*/
		pcdev= &pcdrv_data.pcdev_data[i].cdev ;
		cdev_init(pcdev,&pcd_fops);

		/*Register a device(cdev structure) with VFS */
		pcdev->owner = THIS_MODULE;
		ret = cdev_add(pcdev,device_number,1);
		if(ret < 0){
			pr_err("Cdev add failed");
			goto cdev_del;
		}


		/*populate the sysfs with device information */
		pcdrv_data.device_pcd = device_create(pcdrv_data.class_pcd,NULL,device_number,NULL,"pcdev-%d",i+1);
		if(IS_ERR(pcdrv_data.class_pcd))
		{
			pr_err("Device creation failed\n");
			ret = PTR_ERR(pcdrv_data.class_pcd);
			goto class_del;
		}
	}


	pr_info("Module init was successful\n");

	return 0;

cdev_del:
class_del:
	for(;i>=0;i--)
	{
		device_destroy(pcdrv_data.class_pcd,pcdrv_data.device_number+i);
		cdev_del(&pcdrv_data.pcdev_data[i].cdev);
	}
	class_destroy(pcdrv_data.class_pcd);
unreg_chrdev:
	unregister_chrdev_region(pcdrv_data.device_number,NO_OF_DEVICES);
out:
	pr_info("Module insertion failed\n");
	return ret;
}



static void __exit pcd_driver_exit(void)
{
	int i=0;

        for(i=0 ; i < NO_OF_DEVICES ;i++)
        {
                device_destroy(pcdrv_data.class_pcd,pcdrv_data.device_number+i);
                cdev_del(&pcdrv_data.pcdev_data[i].cdev);
        }
        class_destroy(pcdrv_data.class_pcd);
        unregister_chrdev_region(pcdrv_data.device_number,NO_OF_DEVICES);

	pr_info("module unloaded\n");
	return;
}

module_init(pcd_driver_init);
module_exit(pcd_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ME");
MODULE_DESCRIPTION("A pseudo character driver which handles n devices");
MODULE_INFO(board,"beagle bone wireless");
