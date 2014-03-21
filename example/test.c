#include <linux/module.h>

static int __init test_module_init(void) {
	printk(KERN_INFO "test: starting\n");
	return 0;
}

static void __exit test_module_exit(void) {
        printk(KERN_INFO "test: exiting\n");
}

module_init(test_module_init);
module_exit(test_module_exit);
MODULE_LICENSE("Dual BSD/GPL");
