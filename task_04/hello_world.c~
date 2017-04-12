#include <linux/kernel.h>
#include <linux/module.h>

int init_module(void)
{
  printk(KERN_DEBUG "Hello world!\n");
  return 0;
}

void cleanup_module(void)
{
  printk(KERN_DEBUG "Goodbye World\n");
}
