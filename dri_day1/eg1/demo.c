#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
int intval=0;
char *p = NULL;

module_param(intval,int,0664);
MODULE_PARM_DESC(intval,"this is a integer.");
module_param(p,charp,0664);
MODULE_PARM_DESC(p,"this is a char *.");

int array[3] = {};
int num = 3;

module_param_array(array,int,&num,0664);
MODULE_PARM_DESC(array,"this is a int a[].");

char str[128] = {};
//参数estr用于接收字符串，并将字符串赋值个str参数
module_param_string(estr,str,sizeof(str),0664);
MODULE_PARM_DESC(str,"this is a char a[].");

static int __init demo_init(void)
{
	int i = 0;

	printk("%s,%d\n",__func__,__LINE__);
	printk("intval:%d\n",intval);
	printk("p:%s\n",p);

	for(i=0;i<3;i++)
	{
		printk("array[%d] = %d\n",i,array[i]);
	}

	printk("estr = %s\n",str);
	return 0;
}

static void __exit demo_exit(void)
{
	printk("%s,%d\n",__func__,__LINE__);
}

//显式
module_init(demo_init);
module_exit(demo_exit);

