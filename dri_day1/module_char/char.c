#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>

//许可证声明
MODULE_LICENSE("GPL");

//生成字符设备号
dev_t devno;
int major = 250;
int minor = 0;
int count = 1;

//内核使用一个struct cdev的结构体来描述一个字符设备,并且这个结构在内核中已经
//写好了，直接通过头文件引用就行了
struct cdev cdev;

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
struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = demo_open,
	.release = demo_release,
};

static int __init demo_init(void) //static限制函数的作用域
{
	int ret = 0;

	printk("%s,%d\n",__func__,__LINE__);

	//生成设备号
	devno = MKDEV(major,minor);

	//申请注册设备号
	ret = register_chrdev_region(devno,count,"demo");
	if(ret)
	{
		printk("Failed ro register_chrdev_region.\n");
		return ret;
	}

	//对字符设备进行初始化
	cdev_init(&cdev,&fops);

	ret = cdev_add(&cdev,devno,count);
	if(ret)
	{
		printk("%s,%d\n",__func__,__LINE__);
	
		goto err1;
	}
	return 0;
err1:
	unregister_chrdev_region(devno,count);
	return ret;
}

static void __exit demo_exit(void)
{
	printk("%s,%d\n",__func__,__LINE__);

	cdev_del(&cdev);
	unregister_chrdev_region(devno,count);
}
//显式加载函数
module_init(demo_init);
//显式卸载函数
module_exit(demo_exit);
