#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <asm/uaccess.h>
#include <asm/io.h>

//许可证声明
MODULE_LICENSE("GPL");

struct resource *res;

void __iomem * gpf3con;
void __iomem * gpf3dat;

int demo_probe(struct platform_device *pdevice)
{
	unsigned int dat = 0;

	printk("%s,%d\n",__func__,__LINE__);
	printk("demo_probe ok!\n");

	//获取平台总线上的设备的资源，以第三个参数来确定具体的那个资源
	res = platform_get_resource(pdevice,IORESOURCE_MEM,0);
	if(res == NULL)
	{
		printk("Failed ro platform_get_resource.\n");
		return -1;
	}
	printk("start:%#x --> %#x\n",(unsigned int)res->start,(unsigned int)res->end);

	//对获取的资源进行处理
	gpf3con = ioremap(res->start,4);
	if(gpf3con == NULL)
	{
		printk("Failed to ioremap.\n");
		return -1;
	}
	gpf3dat = ioremap(res->end,4);
	if(gpf3dat == NULL)
	{
		printk("Failed ro ioremap.\n");
		return -1;
	}
	
	dat = readl(gpf3con);
	dat = (dat & (~(0xf << 16))) | (0x1 << 16);
	writel(dat,gpf3con);

	dat = readl(gpf3dat);
	dat = dat | (0x1 << 4);
	writel(dat,gpf3dat);

	printk("led4 on.\n");
	return 0;
}

//在卸载的时候调用此函数，将LED4关掉
int demo_remove(struct platform_device *pdevice)
{
	unsigned int dat;
	printk("%s,%d\n",__func__,__LINE__);
	
	dat = readl(gpf3dat);
	dat = dat & (~(0x1 << 4));
	writel(dat,gpf3dat);
  
	printk("led4 off.\n");
	return 0;
}

struct platform_device_id ids[] = {
	{.name = "led",},
	{/*Nothing to be done.*/},
};

struct platform_driver pdriver ={
	//探测函数
	.probe = demo_probe,
	//卸载的时候，调用demo_remove函数
	.remove = demo_remove,
	//本驱动的名字
	.driver = {
		.name = "demo"
	},
	//先通过匹配表ids进行匹配，匹配成功后调用探测函数demo_probo
	.id_table = ids,
};

module_platform_driver(pdriver);
