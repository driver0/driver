#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <asm/io.h>

MODULE_LICENSE("GPL");

#define GPF3CON 0X114001E0
#define GPF3DAT 0X114001E4

void __iomem * gpf3con;
void __iomem * gpf3dat;

static int __init demo_init(void)
{
	unsigned int dat = 0;

	printk("%s,%d\n",__func__,__LINE__);

	gpf3con = ioremap(GPF3CON,4);
	if(gpf3con == NULL)
	{
		printk("Failed ro ioremap.\n");
		return -1;
	}
	gpf3dat = ioremap(GPF3DAT,4);
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

static void __exit demo_exit(void)
{
	unsigned int dat = 0;

	printk("%s,%d\n",__func__,__LINE__);
	
	dat = readl(gpf3dat);
	dat = dat & (~(0x1 << 4));
	writel(dat,gpf3dat);

	printk("led4 off.\n");
}
module_init(demo_init);
module_exit(demo_exit);
