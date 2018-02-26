#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/time.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");

struct timer_list timer;

void time_func(unsigned long data)
{
	printk("time out.jiffies:%ld\n",jiffies);
	mod_timer(&timer,jiffies+5*HZ);
}
static int __init demo_init(void)
{
	printk("%s,%d\n",__func__,__LINE__);

	printk("jiffies:%ld\n",jiffies);
	printk("HZ:%d\n",HZ);
	printk("jiffies:%ld\n",jiffies);
	timer.function = time_func;
	timer.expires = jiffies+5*HZ;
	init_timer(&timer);
	
	add_timer(&timer);
	return 0;
}
static void __exit demo_exit(void)
{
	printk("%s,%d\n",__func__,__LINE__);

	del_timer(&timer);
}
module_init(demo_init);
module_exit(demo_exit);
