#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");

struct node{
	int a;
	char ch;
	short b;
	int d;
};

static int __init demo_init(void)
{
	struct node temp;
	struct node *p;

	temp.a = 100;
	temp.ch = 'A';
	temp.b = 20;
	temp.d = 300;

	printk("%s,%d\n",__func__,__LINE__);

	printk("&temp:%p\n",&temp);
	
	p = container_of(&(temp.b),struct node,b);

	printk("p:%p a:%d ch:%c b:%d d:%d\n",p,p->a,p->ch,p->b,p->d);
	return 0;
}

static void __exit demo_exit(void)
{
	printk("%s,%d\n",__func__,__LINE__);
}
module_init(demo_init);
module_exit(demo_exit);
