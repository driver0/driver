#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm-generic/uaccess.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/io.h>
#include "cmd.h"

//许可证声明
MODULE_LICENSE("GPL");

//LED2
#define GPX1CON 0X11000C20
#define GPX1DAT 0X11000C24
//led1
#define GPX2CON 0X11000C40
#define GPX2DAT 0X11000C44
//LED3\LED4
#define GPF3CON 0X114001E0
#define GPF3DAT 0X114001E4
unsigned int * gpx2con;
unsigned int  * gpx2dat;

unsigned int * gpx1con;
unsigned int * gpx1dat;

unsigned int * gpf3con;
unsigned int * gpf3dat;

//生成字符设备号
dev_t devno;
int major = 0;
int minor = 0;
int count = 1;

struct cdev *pdev;
struct device *pdevice;
struct class *mclass;

int fs4412_led_ioremap(void)
{
	//将物理地址映射为虚拟地址
	 gpx1con = ioremap(GPX1CON,4);
	 if(gpx1con == NULL)
	 {
		 printk("Failed ro ioremap.\n");
		 return -1;
	 }
	 gpx1dat = ioremap(GPX1DAT,4);
	 if(gpx1dat == NULL)
	 {
		 printk("Failed ro ioremap.\n");
		 return -1;
	 }

	 gpx2con = ioremap(GPX2CON,4);
	 if(gpx2con == NULL)
	 {
		 printk("Failed ro ioremap.\n");
		 return -1;
	 }
	 gpx2dat = ioremap(GPX2DAT,4);
	 if(gpx2dat == NULL)
	 {
		 printk("Failed ro ioremap.\n");
		 return -1;
	 }
	
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

	 return 0;
}

int fs4412_led_iounmap(void)
{
	iounmap(gpx1con);
	iounmap(gpx1dat);

	iounmap(gpx2con);
	iounmap(gpx2dat);

	iounmap(gpf3con);
	iounmap(gpf3dat);

	return 0;
}

int fs4412_led_init(void)
{
	//LED2,GPX1_0
	writel((readl(gpx1con)&(~0xf))|0x1,gpx1con);

	//LED1,GPX2_7
	writel((readl(gpx2con) & (~(0xf << 28))) | (0x1 << 28),gpx2con);

	//LED3,LED4   GPF3_4,GPF3_5 //[16-19],[20-23] 
	writel(((readl(gpf3con) & (~(0xff << 16))) | (0x11 << 16)),gpf3con);

	//ALE LED OFF
	writel(readl(gpx2dat) & (~(1 << 7)),gpx2dat);
	writel(readl(gpx1con) & (~1),gpx1dat);
	writel(readl(gpf3dat) & (~(0x3 << 4)),gpf3dat);

	return 0;
}

int fs4412_led_on(int num)
{
	switch(num)
	{
	case 1://LED1 GPX2_7 1
		writel(readl(gpx2dat) | (1 << 7),gpx2dat);
		break;
	case 2://LED2
		writel(readl(gpx1dat) | 1,gpx1dat);
		break;
	case 3://LED3
		writel(readl(gpx1dat) | (1 << 4),gpf3dat);
		break;
	case 4://LED4
		writel(readl(gpf3dat) | (1 << 5),gpf3dat);
		break;
	}

	return 0;
}

int fs4412_led_off(int num)
{
	switch(num)
	{
	case 1://led1
		writel(readl(gpx2dat) & (~(1 << 7)),gpx2dat);
		break;
	case 2://led2
		writel(readl(gpx1con) & (~1),gpx1dat);
		break;
	case 3://led3
		writel(readl(gpf3dat) & (~(0x1 << 4)),gpf3dat);
		break;
	case 4://LED4
		writel(readl(gpf3dat) & (~(0x1 << 4)),gpf3dat);
		break;
	}

	return 0;
}

//文件操作集合
int demo_open(struct inode *inode,struct file *filp)
{
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

int demo_release(struct inode *inode,struct file *filp)
{
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

//ioctl() --> ... --> demo_ioctl()
long demo_ioctl(struct file *filp,unsigned int cmd,unsigned long arg)
{
	//num 灯的编号
	int num = 0;
	if(copy_from_user(&num,(void *)arg,sizeof(int)))
	{
		printk("Failed ro copy_from_user.\n");
		return -1;
	}
	if(num < 1 || num > 4)
	{
		printk("Invalid num.\n");
		return -EINVAL;
	}
	switch(cmd)
	{
	case LED_ON:
		fs4412_led_on(num);
		break;
	case LED_OFF:
		fs4412_led_off(num);
		break; 
	default:
		printk("Invalid data.\n");
		return -1;
	}
	return 0;
}
struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = demo_open,
	.release = demo_release,
	.unlocked_ioctl = demo_ioctl,
};

static int __init demo_init(void) //static限制函数的作用域
{
	int ret = 0;

	printk("%s,%d\n",__func__,__LINE__);

	ret = alloc_chrdev_region(&devno,minor,count,"demo");
	if(ret)
	{
		printk("Failed to alloc_chrdev_region.\n");
		return ret;
	}

	printk("devno:%d, major:%d, minor:%d\n",devno,major,minor);

	pdev = cdev_alloc();
	if(pdev == NULL)
	{
		printk("Failed ro cdev_alloc.\n");
		ret = -ENOMEM;
		goto err;
	}
	//对字符设备进行初始化
	cdev_init(pdev,&fops);

	ret = cdev_add(pdev,devno,count);
	if(ret)
	{
		printk("Failed ro cdev_add");
		goto err1;
	}

	mclass = class_create(THIS_MODULE,"myclass");
	if(IS_ERR(mclass))
	{
		ret = PTR_ERR(mclass);
		goto err2;
	}
	pdevice = device_create(mclass,NULL,devno,NULL,"led");
	if(IS_ERR(pdevice))
	{
		ret = PTR_ERR(pdevice);
		goto err3;
	}

	//led remap
	fs4412_led_ioremap();

	//led init,off all led
	fs4412_led_init();

	return 0;

err3:
	class_destroy(mclass);
err2:
	cdev_del(pdev);
err1:
	kfree(pdev);
err:
	unregister_chrdev_region(devno,count);
	return ret;
}

static void __exit demo_exit(void)
{
	printk("%s,%d\n",__func__,__LINE__);

	fs4412_led_iounmap();

	device_destroy(mclass,devno);
	class_destroy(mclass);
	cdev_del(pdev);
	kfree(pdev);
	unregister_chrdev_region(devno,count);
}
//显式加载函数
module_init(demo_init);
//显式卸载函数
module_exit(demo_exit);
