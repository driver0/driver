#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/of.h>
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/workqueue.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");

int irqno;

struct work_struct workqueue;

void msleep(unsigned int msecs)
{
	unsigned long timeout = msecs_to_jiffies(msecs)+1;
	while(timeout)
		timeout = schedule_timeout_uninterruptible(timeout);
}
void workqueue_func(struct work_struct *work)
{
	printk("Before workqueue_func.\n");
	msleep(10000);
	printk("After workqueue_func.\n");
}

irqreturn_t key_handler(int irqno,void *dev_id)
{
	printk("key ok!--------------------------\n");
	schedule_work(&workqueue);
	printk("key ok!++++++++++++++++++++++++++\n");
	return IRQ_HANDLED;
}
int demo_probe(struct platform_device *pdevice)
{
	int ret = 0;

	printk("demo_probe ok!\n");

	irqno = platform_get_irq(pdevice,0);
	if(irqno < 0)
	{
		printk("Failed ro platform_get_irq.\n");
		return -1;
	}

	printk("irqno:%d\n",irqno);

	INIT_WORK(&workqueue,workqueue_func);

	ret = request_irq(irqno,key_handler,IRQF_DISABLED | IRQF_TRIGGER_FALLING,"key",NULL);
	if(ret)
	{
		printk("Failed ro request_irq.\n");
		return ret;
	}

	return 0;
}
int demo_remove(struct platform_device * pdevice)
{
	printk("%s,%d\n",__func__,__LINE__);

	free_irq(irqno,NULL);
	return 0;
}
//要先匹配才能做其他事，这里用的是设备树匹配，所以要先建立设备树
//匹配表
struct of_device_id idts[] = {
	{.compatible = "fs4412,key"},
	{/*Nothing ro done.*/},
};
struct platform_driver pdriver = {
	.probe = demo_probe,
	.remove = demo_remove,
	.driver = {
		.name = "demo",
		//设备树匹配表
		.of_match_table = idts,
	},
};
module_platform_driver(pdriver);
