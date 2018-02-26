#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/of.h>

MODULE_LICENSE("GPL");

struct resource *res1;
struct resource *res2;
void __iomem *gpf3con;
void __iomem *gpf3dat;

int demo_probe(struct platform_device *pdevice)
{
	printk("demo_probe ok!\n");

	res1 = platform_get_resource(pdevice,IORESOURCE_MEM,0);
	if(res1 == NULL)
	{
		printk("Failed to platform_get_resource.\n");
		return -1;
	}
	printk("gpf3con:%#x \n",res1->start);

	res2 = platform_get_resource(pdevice,IORESOURCE_MEM,1);
	if(res2 == NULL)
	{
		printk("Failed to platform_get_resource.\n");
		return -1;
	}
	printk("gpf3dat:%#x \n",res2->start);

	gpf3con = ioremap(res1->start,4);
	if(gpf3con == NULL)
	{
		printk("Failed to ioremap.\n");
		return -1;
	}

	gpf3dat = ioremap(res2->start,4);
	if(gpf3dat == NULL)
	{
		printk("Failed to ioremap.\n");
		return -1;
	}

	writel((readl(gpf3con) & (~(0xf << 20))) | (0x1 << 20),gpf3con);
	writel(readl(gpf3dat) | (0x1 << 5),gpf3dat);
	return 0;
}
int demo_remove(struct platform_device * pdevice)
{
	printk("%s,%d\n",__func__,__LINE__);

	writel(readl(gpf3dat) & (~(0x1 << 5)),gpf3dat);
	return 0;
}
//要先匹配才能做其他事，这里用的是设备树匹配，所以要先建立设备树
//匹配表
struct of_device_id idts[] = {
	{.compatible = "farsight,led5"},
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
