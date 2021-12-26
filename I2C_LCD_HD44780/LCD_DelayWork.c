/*
 * Kernel timer
 */

#define pr_fmt(fmt) "%s:%s: " fmt, KBUILD_MODNAME, __func__
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/workqueue.h> /* for work queue */
# include "./Arduino_HD44780Driver/HD44780U_I2C_Burton.h"



static struct delayed_work initiate_delay_work;
static int data=0;

static void delayed_work(struct work_struct *work)
{
	pr_info("delay work %d\n",data++);
	scrollDisplayRight();
    mod_delayed_work(system_wq, &initiate_delay_work, msecs_to_jiffies(2000));
}

int my_delayed_work_init( void )
{
    INIT_DELAYED_WORK(&initiate_delay_work, delayed_work);
    schedule_delayed_work(&initiate_delay_work, msecs_to_jiffies(2000));
    return 0;
}

void my_delayed_work_exit( void )
{
	cancel_delayed_work_sync(&initiate_delay_work);
    return;
}




// module_init(ktimer_init);
// module_exit(ktimer_exit);

// MODULE_AUTHOR("Rodolfo Giometti");
// MODULE_DESCRIPTION("Kernel timer");
// MODULE_LICENSE("GPL");
