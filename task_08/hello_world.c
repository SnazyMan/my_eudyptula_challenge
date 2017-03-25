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
 * jiffies read only by any user, when read should return the current value of jiffies kernel timer
 * foo writeable to root, but readable to anyone; when written to, the value must be stored, up to one  * page of data. WHen read, which can be done by any user, the value must be returned that is stored in
 * it. Properly handle the fact that someone could be reading from the file while someone else is
 * writing to it (locking hint)
 * when the module is unloaded, all of the debugfs files are cleaned up, and any memory allocated is
 * freed
 */

#define DRIVER_NAME "snazy_misc_device"
#define NODE_NAME "eudyptula"
#define ID_BUF_LEN 13

//directory /sys/kernel/debug/eudyptula
static struct dentry *snazy_dir = 0;

unsigned long snazy_page_address;

DEFINE_MUTEX(snazy_mutex);

static ssize_t snazy_misc_device_read(struct file *file, char __user  *buff, size_t count, loff_t *pos)
{
        char content[ID_BUF_LEN] = "9850aeb5fe79";

        if(copy_to_user(buff, content, count))
                return -EFAULT;

        *pos += count;
        return count;
}

static ssize_t snazy_misc_device_write(struct file *file, const char __user *buff, size_t count, loff_t *pos)
{

        char content[ID_BUF_LEN];
        char id[ID_BUF_LEN] = "9850aeb5fe79";

        if(count > ID_BUF_LEN)
                return -EINVAL;

        if(copy_from_user(content, buff, count))
                return -EFAULT;

        if(strcmp(content,id) != 0)
                return -EINVAL;

        *pos+= count;
        return count;
}

static const struct file_operations snazy_misc_device_fops = {
        .owner = THIS_MODULE,
        .read  = snazy_misc_device_read,
        .write = snazy_misc_device_write,
};

static ssize_t snazy_jiffies_read(struct file *file, char __user  *buff, size_t count, loff_t *pos)
{
        unsigned long content = jiffies;

        if(copy_to_user(buff, &content, count))
                return -EFAULT;

        *pos += count;
        return count;
}

static const struct file_operations snazy_jiffies_fops = {
        .owner = THIS_MODULE,
        .read = snazy_jiffies_read,
};

static ssize_t snazy_foo_read(struct file *file, char __user *buff, size_t count, loff_t *pos)
{
        mutex_lock(&snazy_mutex);
        if(copy_to_user(buff, snazy_page_address, count))
                return -EFAULT;
        mutex_unlock(&snazy_mutex);

        *pos += count;
        return count;
}

static ssize_t snazy_foo_write(struct file *file, const char __user *buff, size_t count, loff_t * pos)
{
        mutex_lock(&snazy_mutex);
        if(copy_from_user(snazy_page_address, buff, count))
                return -EFAULT;
        mutex_unlock(&snazy_mutex);

        *pos += count;
        return count;
}

static const struct file_operations snazy_foo_fops = {
        .owner = THIS_MODULE,
        .read = snazy_foo_read,
        .write = snazy_foo_write,
};

static int __init snazy_init(void)
{
        struct dentry *id, *jiffies, *foo;

        snazy_dir = debugfs_create_dir(NODE_NAME, 0);
        if(!snazy_dir){
                printk(KERN_ERR "debugfs_eudyptula: failed to create directory");
                return -1;
        }

        id = debugfs_create_file("id", 0600, snazy_dir, 0, &snazy_misc_device_fops);
        if(!id){
                printk(KERN_ERR "debugfs_eudpytula/id: failed to create file");
                return -1;
        }

        jiffies = debugfs_create_file("jiffies", 0444,snazy_dir, 0, &snazy_jiffies_fops);
        if(!jiffies){
                printk(KERN_ERR "debugfs_eudpytula/jiffies: failed to create file");
                return -1;
        }

        foo = debugfs_create_file("foo", 0644, snazy_dir, 0, &snazy_foo_fops);
        if(!foo){
                printk(KERN_ERR "debugfs_eudpytula/jiffies: failed to create file");
                return -1;
        }

        //allocate page of data for foo file
        snazy_page_address = __get_free_page(GFP_KERNEL);
        if(!snazy_page_address)
                return -ENOMEM;

        return 0;
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
