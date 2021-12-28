# include <linux/module.h>
# include <linux/kernel.h>
# include <linux/init.h>
# include <linux/i2c.h>
# include <linux/string.h>
# include <linux/delay.h>
# include "./Arduino_HD44780Driver/Wrapper_Function.h"
# include "./Arduino_HD44780Driver/HD44780U_I2C_Burton.h"
#include "LCD_DelayWork.h"
#include "LCD_Char.h"

//    1、void ndelay(unsigned long nsecs);         纳秒级：1/10^-10
//    2、void udelay(unsigned long usecs);         微秒级: 1/10^-6
//    3、void mdelay(unsigned long msecs);         毫秒级：1/10^-3
static struct i2c_client *myClient=NULL;
//----------------wrapper function-----------------------//

void Wrapper_Delay(uint32_t value)
{
    mdelay(value);
    //msleep(value);
}
void Wrapper_delayMicroseconds(uint32_t value)
{
    udelay(value);
    //msleep( (value+999)/1000);
}
void Wrapper_WriteOneByte_I2C(uint8_t value)
{
    i2c_smbus_write_byte(myClient,value);
}


static int dummy_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    int i=0;
    int ret = 0;
    myClient = client;
    pr_info("Dummy device is being probed.\n");
    char *info = "Hello, Arduino!\n";
    // for (i = 0; i < strlen(info); i++) {
    // 	i2c_smbus_write_byte(client, info[i]);
    // }
    LCD_init();
    LCD_init();
    // Print a message to the LCD.
    backlight();
    setCursor(3,0);
    WriteMessage("Hello, world!");
    setCursor(2,1);
    WriteMessage("Ywrobot Arduino!");
    setCursor(0,2);
    WriteMessage("Arduino LCM IIC 2004");
    setCursor(2,3);
    WriteMessage("Power By Ec-yuan!");
    setCursor(0,0);

    my_delayed_work_init();
    ret = char_probe();
    if( ret )
    {
        return ret;
    }
        
    return 0;      
}

static int dummy_remove(struct i2c_client *client)
{
    char_remove();
    my_delayed_work_exit();
    pr_info("Dummy device is removing.\n");
    return 0;
}

static struct of_device_id dummy_id_tables [] = {
    { .compatible="hd44780,lcd", },
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