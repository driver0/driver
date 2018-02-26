#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/of.h>

MODULE_LICENSE("GPL");

int demo_probe(struct platform_device *pdevice)
{
	struct device_node *np;
	struct property * proper;
	int len = sizeof(int);
	const char *out_string;
	int ret = 0;

	printk("demo_probe ok!\n");

	//通过路径找设备树节点，得到节点的指针
	np = of_find_node_by_path("/xxx");
	if(np == NULL)
	{
		printk("Failed ro of_find_node_by_path.");
		return -1;
	}

	//已知设备树节点的指针，获取设备节点的属性键值对的值
	proper = of_find_property(np,"b",&len);
	if(proper ==NULL)
	{
		printk("Failed to of_find_property.");
		return -1;
	}

	printk("b:%#x\n",be32_to_cpu(*((unsigned int *)proper->value)));

	//读取一个字符串
	ret = of_property_read_string(np,"a",&out_string);
	if(ret)
	{
		printk("Failed to of_property_read_string.\n");
		return -1;
	}
	printk("a:%s\n",out_string);

	return 0;
}
int demo_remove(struct platform_device * pdevice)
{
	printk("%s,%d\n",__func__,__LINE__);

	return 0;
}
//要先匹配才能做其他事，这里用的是设备树匹配，所以要先建立设备树
//匹配表
struct of_device_id idts[] = {
	{.compatible = "fs4412,xxx"},
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
