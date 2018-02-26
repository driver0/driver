#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>'
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/device.h>

MODULE_LICENSE("GPL");

#define DEVNONAME "demo"

dev_t devno;
int major = 0;
int minor = 0;
int count = 10;

struct cdev *pdev;
struct device *pdevice;
struct class *pclass;

int demo_open(struct inode *inode,struct file * filp)
{
	printk("%s,%d\n",__func__,__LINE__);

	return 0;
}

int demo_release(struct inode *inode,struct file* filp)
{
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = demo_open,
	.release = demo_release,
};

static int __init demo_init(void)
{
	int ret = 0;
	int i = 0;
	int j = 0;

	printk("%s,%d\n",__func__,__LINE__);

	ret = alloc_chrdev_region(&devno,minor,count,DEVNONAME);
	if(ret)
	{
		printk("Failed to alloc_chrdev_region.\n");
		return ret;
	}

	printk("devno:%d major:%d minor:%d \n",devno,MAJOR(devno),MINOR(devno));

	pdev = cdev_alloc();
	if(pdev == NULL)
	{
		printk("Failed to cdev_alloc.\n");
		goto err1;
	}

	cdev_init(pdev,&fops);

	ret = cdev_add(pdev,devno,count);
	if(ret)
	{
		printk("Failed to cdev_add.\n");
		goto err2;
	}

	//自动创建设备节点，代替mknod /dev/demo
	pclass = class_create(THIS_MODULE,"myclass");
	if(IS_ERR(pclass))
	{
		ret = PTR_ERR(pclass);
		goto err3;
	}

	for(i = 0; i < count; i++)
	{
		pdevice = device_create(pclass,NULL,MKDEV(MAJOR(devno),MINOR(devno)+i),NULL,"demo%d",i);
		if(pdevice == NULL)
		{
			printk("Failed to device_create.\n");
			for(j = 0;j < i; j++)
			{
				device_destroy(pclass,MKDEV(MAJOR(devno),MINOR(devno)+j));
			}
			goto err4;
		}
	}
	return 0;
err4:
	class_destroy(pclass);
err3:
	cdev_del(pdev);
err2:
	kfree(pdev);
err1:
	unregister_chrdev_region(devno,count);
	return ret;
}

static void __exit demo_exit(void)
{
	printk("%s,%d\n",__func__,__LINE__);

	device_destroy(pclass,devno);
	class_destroy(pclass);
	cdev_del(pdev);
	kfree(pdev);
	unregister_chrdev_region(devno,count);
}

module_init(demo_init);
module_exit(demo_exit);
