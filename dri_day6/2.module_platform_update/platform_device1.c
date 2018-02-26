#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>

//许可证声明
MODULE_LICENSE("GPL");

struct resource res[2] = {
	[0] = {
		.start = 0x10009000,
		.end   = 0x10009008,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = 0x20003000,
		.end   = 0x20003004,
		.flags = IORESOURCE_MEM,
	},
};

void demo_release(struct device *dev)
{
	printk("%s,%d\n",__func__,__LINE__);
}
struct platform_device pdevice ={
	.name = "demo2",
	.num_resources = ARRAY_SIZE(res),
	.resource = res,
	.dev = {
		.release = demo_release,
	},
};

static int __init demo_init(void) //static限制函数的作用域
{
	printk("%s,%d\n",__func__,__LINE__);

	platform_device_register(&pdevice);

	return 0;
}

static void __exit demo_exit(void)
{
	printk("%s,%d\n",__func__,__LINE__);
	platform_device_unregister(&pdevice);
}
//显式加载函数
module_init(demo_init);
//显式卸载函数
module_exit(demo_exit);
