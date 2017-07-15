#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/wait.h>
#include <linux/init.h>
#include <linux/kthread.h>


#define DRIVER_NAME "snazy_misc_device"
#define NODE_NAME "eudyptula"
#define ID_BUF_LEN 13
static char *eudyptula_id = "9850aeb5fe79";

DECLARE_WAIT_QUEUE_HEAD(wee_wait);
static bool condition = false;
struct task_struct *snazy_thread;

int snazy_threadfn(void *data)
{
	while (1) {
		wait_event_interruptible_timeout(wee_wait, condition == true, HZ);
		if (kthread_should_stop())
			return 0;
	}
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
	.write = snazy_misc_device_write,
};

static struct miscdevice snazy_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = DRIVER_NAME,
	.fops  = &snazy_misc_device_fops,
	.nodename = NODE_NAME,
	.mode = 0222,
};

static int __init snazy_init(void)
{
	int ret;

	ret = misc_register(&snazy_misc_device);
	if (ret)
	        return -1;

	snazy_thread = kthread_run(snazy_threadfn, NULL, "eudyptula");
	if (snazy_thread == NULL)
		return -1;

	return 0;
}

static void __exit snazy_exit(void)
{
	int ret;
	
	ret = kthread_stop(snazy_thread);
	misc_deregister(&snazy_misc_device);
}

module_init(snazy_init);
module_exit(snazy_exit);
//module_misc_device(snazy_misc_device);

MODULE_AUTHOR("Tyler Olivieri - Snazyman");
MODULE_DESCRIPTION("task_17 of eudyptula challenge");
MODULE_LICENSE("GPL");
