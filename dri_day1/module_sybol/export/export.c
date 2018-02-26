#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
//因为sym.h在上一级目录中，需要包含一下才能用
#include "../sym.h"

//模块许可证声明
MODULE_LICENSE("GPL");

int func(void)
{
	printk("this is a export sym test.\n");
	return 0;
}

struct obj o = {
	.a = 100,
	.show =func,
};
//模块可以使用如下宏导出符号到内核符号表
//EXPORT_SYMBOL（符号表）
//EXPORT_SYMBOL_GPL(符号名)
EXPORT_SYMBOL_GPL(o);

static int __init demo_init(void)
{
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

static void __exit demo_exit(void)
{
	printk("%s,%d\n",__func__,__LINE__);
}

//显示加载module_init-->demo_init
module_init(demo_init);
//显示卸载module_exit-->demo_exit
module_exit(demo_exit);
