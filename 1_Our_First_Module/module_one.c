#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */



MODULE_LICENSE("GPL");
MODULE_AUTHOR("GrgoMariani");
MODULE_DESCRIPTION("This module does absolutely nothing other than logging init and exit");
MODULE_VERSION("0.01");



static char *name = "MEGASUPERMODULE";
module_param(name, charp, S_IRUGO); ///< Param description |  charp = char ptr | S_IRUGO = can be read/not changed
MODULE_PARM_DESC(name, " The name to display in /var/log/kern.log");

static int somenumber = 13;
module_param(somenumber, int, 0);  ///< Param description | 0 - permissions
MODULE_PARM_DESC(somenumber, " Some Number");



static int __init module_one_init(void)
{
	printk(KERN_INFO "Module initialized with name %s. Some number is %d. \n", name, somenumber);
	return 0;   // 0 is returned when everything went OK.
}


static void __exit module_one_exit(void)
{
	printk(KERN_INFO "Cleaning up the code for module %s. Some number was %d. \n", name, somenumber);
}



module_init(module_one_init);
module_exit(module_one_exit);
