#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>
#include <linux/mutex.h>

/*
 * Create debugfs directory called "eudyptula"
 * Create 3 virtual files in eudyptula called  "id", "jiffies", and "foo"
 * id operates just like task 06 - must be readable and writeable to any user
 * jiffies read only by any user, when read should return the current value
 * of jiffies kernel timer
 * foo writeable to root, but readable to anyone; when written to,
 * the value must be stored, up to one	* page of data. When read,
 * which can be done by any user, the value must be returned that is stored in
 * it. Properly handle the fact that someone could be reading
 * from the file while someone else is
 * writing to it (locking hint)
 * when the module is unloaded, all of the debugfs files are cleaned up,
 * and any memory allocated is
 * freed
 */

#define DRIVER_NAME "snazy_misc_device"
#define NODE_NAME "eudyptula"
#define ID_BUF_LEN 13
static char *eudyptula_id = "9850aeb5fe79";

//directory /sys/kernel/debug/eudyptula
static struct dentry *snazy_dir;

unsigned long snazy_page_address;

DEFINE_MUTEX(snazy_mutex);

static ssize_t snazy_misc_device_read(struct file *file, char __user  *buff, size_t count, loff_t *pos)
{
	return simple_read_from_buffer(buff, count, pos, eudyptula_id, ID_BUF_LEN);
}

static ssize_t snazy_misc_device_write(struct file *file, const char __user *buff, size_t count, loff_t *pos)
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

static int __init snazy_init(void)
{
	struct dentry *id;

	snazy_dir = debugfs_create_dir(NODE_NAME, 0);
	if (!snazy_dir)
		goto err_out;

	id = debugfs_create_file("id", 0666, snazy_dir, 0, &snazy_misc_device_fops);
	if (!id)
		goto err_out;

	snazy_page_address = __get_free_page(GFP_KERNEL);
	if (!snazy_page_address)
		return -ENOMEM;

	return 0;

err_out:
	pr_err("Failed to initialize debug_fs directory and files\n");
	return -1;
}

static void __exit snazy_exit(void)
{
	debugfs_remove_recursive(snazy_dir);
	free_page(snazy_page_address);
}

module_init(snazy_init);
module_exit(snazy_exit);

MODULE_AUTHOR("Tyler Olivieri - Snazyman");
MODULE_DESCRIPTION("task_08 of eudyptula challenge");
MODULE_LICENSE("GPL");

