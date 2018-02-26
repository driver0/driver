//本设备是LED4，并包含与其相关的寄存器地址到其资源里
//驱动可通过获取资源函数获得LED4设备的资源，并对其进行相应的处理
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>

//许可证声明
MODULE_LICENSE("GPL");

//宏定义LED4对应寄存器的地址，为地址映射做准备
#define GPF3CON 0X114001E0
#define GPF3DAT 0X114001E4

//设备所拥有的资源初始化，这里我们用的资源是LED4对应寄存器的地址
//我们将通过驱动来获取它，并进行相应的处理
struct resource res[1] = {
	[0] = {
		.start = GPF3CON,
		.end   = GPF3CON+4,
		.flags = IORESOURCE_MEM,
	},
};
//消除卸载时出现的警告
void demo_release(struct device *dev)
{
	printk("%s,%d\n",__func__,__LINE__);
}

struct platform_device pdevice ={
	.name = "led",  //设备的名字为led，用与和驱动匹配的条件
	.num_resources = ARRAY_SIZE(res),//资源的数量
	.resource = res,   //资源
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
