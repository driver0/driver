#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>

//许可证声明
MODULE_LICENSE("GPL");

struct resource *res;

int demo_probe(struct platform_device *pdevice)
{
	printk("%s,%d\n",__func__,__LINE__);
	printk("demo_probe ok!\n");

	res = platform_get_resource(pdevice,IORESOURCE_MEM,0);
	if(res == NULL)
	{
		printk("Failed ro platform_get_resource.\n");
		return -1;
	}
	printk("start:%#x --> %#x\n",(unsigned int)res->start,(unsigned int)res->end);

	res = platform_get_resource(pdevice,IORESOURCE_MEM,1);
	if(res == NULL)
	{
		printk("Failed ro platform_get_resource.\n");
		return -1;
	}
	printk("start:%#x --> %#x\n",(unsigned int)res->start,(unsigned int)res->end);

	return 0;
}

int demo_remove(struct platform_device *pdevice)
{
	printk("%s,%d\n",__func__,__LINE__);

	return 0;
}
struct platform_driver pdriver ={
	.probe = demo_probe,
	.remove = demo_remove,
	.driver = {
		.name = "demo"
	},
};

static int __init demo_init(void) //static限制函数的作用域
{
	printk("%s,%d\n",__func__,__LINE__);

	platform_driver_register(&pdriver);

	return 0;
}

static void __exit demo_exit(void)
{
	printk("%s,%d\n",__func__,__LINE__);

	platform_driver_unregister(&pdriver);
}
//显式加载函数
module_init(demo_init);
//显式卸载函数
module_exit(demo_exit);
