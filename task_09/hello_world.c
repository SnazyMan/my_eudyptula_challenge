#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>
#include <linux/mutex.h>
#include <linux/kobject.h>

/*
 * Take code from task 08 (debugfs) and move it to sysfs
 * Place "eudyptula" directory under /sys/kernel/
 * This is a combination of task_08 and samples/kobject/kobj-example.c
 */

#define DRIVER_NAME "snazy_misc_device"
#define NODE_NAME "eudyptula"
#define ID_BUF_LEN 13

static unsigned long snazy_page_address;

static struct kobject *snazy_kobj;

unsigned int snazy_foo;

DEFINE_MUTEX(snazy_mutex);

static ssize_t snazy_id_show(struct kobject * kobj, struct kobj_attribute *attr, char *buff)
{
        char content[ID_BUF_LEN] = "9850aeb5fe79";

        return sprintf(buff, "%s\n", content);
}

static ssize_t snazy_id_store(struct kobject * kobj, struct kobj_attribute * attr, const char *buff, size_t count)
{

        char content[ID_BUF_LEN];
        char id[ID_BUF_LEN] = "9850aeb5fe79";

        if(count > ID_BUF_LEN)
                return -EINVAL;

        if(copy_from_user(content, buff, count))
                return -EFAULT;

        if(strcmp(content,id) != 0)
                return -EINVAL;

        return count;
}

static ssize_t snazy_jiffies_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff)
{
        return sprintf(buff, "%d\n", jiffies);
}

static ssize_t snazy_jiffies_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buff, size_t count)
{
        return 0;
}

static ssize_t snazy_foo_show(struct kobject *kobj, struct kobj_attribute * attr, char *buff)
{
        int count;

        mutex_lock(&snazy_mutex);
        count = sprintf(buff, "%d\n", snazy_foo);
        mutex_unlock(&snazy_mutex);

        return count;
}

static ssize_t snazy_foo_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buff, size_t count)
{
        int status;

        mutex_lock(&snazy_mutex);
        status = kstrtoint(buff, 10, &snazy_foo);
        if(status < 0)
                return status;
        mutex_unlock(&snazy_mutex);

        return count;
}

static struct kobj_attribute snazy_id_attribute =
        __ATTR(id, 0644, snazy_id_show, snazy_id_store);

static struct kobj_attribute snazy_jiffies_attribute =
        __ATTR(jiffies, 0644, snazy_jiffies_show, snazy_jiffies_store);

static struct kobj_attribute snazy_foo_attribute =
        __ATTR(foo, 0644, snazy_foo_show, snazy_foo_store);

static struct attribute * snazy_attrs[] = {
        &snazy_id_attribute.attr,
        &snazy_jiffies_attribute.attr,
        &snazy_foo_attribute.attr,
        NULL,
};

static struct attribute_group snazy_attr_group = {
        .attrs = snazy_attrs,
};

static int __init snazy_init(void)
{
        int status;

        snazy_kobj = kobject_create_and_add(NODE_NAME, kernel_kobj);
        if(!snazy_kobj)
                return -ENOMEM;

        status = sysfs_create_group(snazy_kobj, &snazy_attr_group);
        if(status)
                kobject_put(snazy_kobj);

        return status;
}

static void __exit snazy_exit(void)
{
        kobject_put(snazy_kobj);
}

module_init(snazy_init);
module_exit(snazy_exit);

MODULE_AUTHOR("Tyler Olivieri - Snazyman");
MODULE_DESCRIPTION("task_08 of eudyptula challenge");
MODULE_LICENSE("GPL");
