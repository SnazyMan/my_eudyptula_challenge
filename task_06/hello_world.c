#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/string.h>

/*
 * Misc char driver w/ dynamic minor number, read & write methods
 * Misc device should show up in /dev/eudyptula
 * When read, the id "9850aeb5fe79" will be returned
 * When written to, the data sent to kernel must be checked, if it is the id,
 * return success, otherwise return "invalid value" (EINVAL?)
 * error value (errno?)
 * device should be registered when module is loaded and unregistered when
 * unloaded
 */

#define DRIVER_NAME "snazy_misc_device"
#define NODE_NAME "eudyptula"
#define ID_BUF_LEN 13
static char *eudyptula_id = "9850aeb5fe79";

static ssize_t snazy_misc_device_read(struct file *file, char __user  *buff,
				      size_t count, loff_t *pos)
{
	return simple_read_from_buffer(buff, count, pos, eudyptula_id, ID_BUF_LEN);
}

static ssize_t snazy_misc_device_write(struct file *file,
				       const char __user *buff,
				       size_t count, loff_t *pos)
{
	int ret;
	char content[ID_BUF_LEN];

	ret = simple_write_to_buffer(content, ID_BUF_LEN, pos, buff, count);
	if (ret < 0)
		return ret;
	if (strcmp(content, eudyptula_id) != 0)
		return -EINVAL;
	return ret;
}

static const struct file_operations snazy_misc_device_fops = {
	.owner = THIS_MODULE,
	.read  = snazy_misc_device_read,
	.write = snazy_misc_device_write,
};

static struct miscdevice snazy_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = DRIVER_NAME,
	.fops  = &snazy_misc_device_fops,
	.nodename = NODE_NAME,
};
module_misc_device(snazy_misc_device);

MODULE_AUTHOR("Tyler Olivieri - Snazyman");
MODULE_DESCRIPTION("task_06 of eudyptula challenge");
MODULE_LICENSE("GPL");
