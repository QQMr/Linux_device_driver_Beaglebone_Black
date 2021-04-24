#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/kdev_t.h>
#include<linux/uaccess.h>
#include<linux/platform_device.h>

#undef pr_fmt
#define pr_fmt(fmt) "%s :" fmt,__func__


#define RDONLY 0X01
#define WRONLY 0X10
#define RDWR   0X11




loff_t pcd_lseek(struct file *filp, loff_t offset, int whence)
{
	return -EPERM;
}

ssize_t pcd_read(struct file * filp, char __user *buff, size_t count, loff_t *f_pos)
{
	return 0;
}

ssize_t pcd_write(struct file * filp, const char __user *buff, size_t count, loff_t *f_pos)
{
	return -ENOMEM;
}


int pcd_open (struct inode *inode, struct file *filp)
{
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

/*gets called when the device is removed from the system*/
int pcd_platform_driver_remove(struct platform_device *pdev)
{
	return 0;
}

/*gets called when matched platform device is found*/
int pcd_platform_driver_prob(struct platform_device *pdev)
{
	return 0;
}

struct platform_driver pcd_platform_driver =
{
	.probe = pcd_platform_driver_prob,
	.remove = pcd_platform_driver_remove,
	.driver = {
		.name = "pseudo-char-device"
	}
};


static int __init pcd_platform_driver_init(void)
{
	platform_driver_register(&pcd_platform_driver);
	pr_info("pcd platform loaded\n");

	return 0;
}



static void __exit pcd_platform_driver_exit(void)
{
	platform_driver_unregister(&pcd_platform_driver);
	pr_info("pcd platform driver unloaded\n");

}

module_init(pcd_platform_driver_init);
module_exit(pcd_platform_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ME");
MODULE_DESCRIPTION("A pseudo character driver which handles n devices");
MODULE_INFO(board,"beagle bone wireless");
