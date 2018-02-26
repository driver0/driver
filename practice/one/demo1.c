#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");

//定义一个内核定时器
struct timer_list my_timer;

void my_function(unsigned long data)
{
	printk("time out. jiffies:%ld\n",jiffies);
	mod_timer(&my_timer,jiffies+HZ*5);
}
static int __init demo_init(void)//static:限制函数的作用域，只能在本文件使用
{
	printk("%s,%d\n",__func__,__LINE__);

	printk("jiffies:%ld\n",jiffies);
	printk("HZ:%d\n",HZ);
	printk("jiffies:%ld\n",jiffies);


	//填充这个定时器结构体的对象
	my_timer.expires = jiffies + HZ*5;
	my_timer.function = my_function;

	//初始化定时器
	init_timer(&my_timer);

	//添加定时器
	add_timer(&my_timer);
	return 0;
}
static void __exit demo_exit(void)
{
	printk("%s,%d\n",__func__,__LINE__);

	del_timer(&my_timer);
}
module_init(demo_init);
module_exit(demo_exit);
