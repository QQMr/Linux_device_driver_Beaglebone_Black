# include <linux/module.h>
# include <linux/kernel.h>
# include <linux/init.h>
# include <linux/i2c.h>
# include <linux/string.h>

static int dummy_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    int i=0;
    pr_info("Dummy device is being probed.\n");
    char *info = "Hello, Arduino!\n";
    for (i = 0; i < strlen(info); i++) {
    	i2c_smbus_write_byte(client, info[i]);
    }
    return 0;      
}

static int dummy_remove(struct i2c_client *client)
{
    pr_info("Dummy device is removing.\n");
    return 0;
}

static struct of_device_id dummy_id_tables [] = {
    { .compatible="arduino", },
    { }
};

MODULE_DEVICE_TABLE(of, dummy_id_tables);

static struct i2c_driver dummy_drv = {
    .probe = dummy_probe,
    .remove = dummy_remove,
    .driver = {
    	.name = "dummy device 0.1",
	.owner = THIS_MODULE,
	.of_match_table = dummy_id_tables,
    },
};

module_i2c_driver(dummy_drv);
MODULE_LICENSE("GPL");