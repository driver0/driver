#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>

//许可证声明
MODULE_LICENSE("GPL");
//定义一个硬件资源结构提用与接收从设备资源中获取的资源
struct resource *res;

//探测函数，一旦设备与驱动匹配成功就会调用此函数
int demo_probe(struct platform_device *pdevice)
{
	printk("%s,%d\n",__func__,__LINE__);
	printk("demo_probe ok!\n");

	//获取硬件资源信息函数：
	//pdevice：已经注册到平台总线上的设备
	//IORESOURCE_MEM：平台设备所带的资源的类型
	//0：资源索引（具体体现在数组的下标）
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

//建立匹配表的函数：用于与设备匹配（通过对比表中的名字与设备的名字）
struct platform_device_id ids[] = {
	{.name = "demo1",},
	{.name = "demo2",},
	{/*Nothing to be done.*/},
};

struct platform_driver pdriver ={
	.probe = demo_probe,
	.remove = demo_remove,
	.driver = {
		.name = "demo"
	},
	.id_table = ids,
};
module_platform_driver(pdriver);
//加载/卸载函数的宏
//#define module_platform_driver(__platform_driver)\
//		module_driver(__platform_driver,platform_driver_register,\
//		platform_dirver_unregister)
//----->
//#define module_driver(__driver,__register,__unregister,....)\
//static int __init __driver##_init(void)
//{
//return __register(&(__driver),##__VA_VAGS__);\
//}
//module_init(__driver##_init);\
//static void __exit __driver##_exit(void)
//{
//__unregister(&(__driver),##__VA_VAGS__);\
//}
//module_exit(__driver##_exit);
//##在函数宏中，表示的是字符串的拼接
