#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/string.h>
#include <linux/slab.h>

struct identity {
	char name[20];
	int id;
	bool busy;
	struct list_head list;
};

static LIST_HEAD(identity_list);

static struct kmem_cache *snazy_cache;

int identity_create(char *name, int id)
{
	int ret = 0;
	size_t length;
	struct identity *new_identity = kmem_cache_alloc(snazy_cache, GFP_KERNEL);
	if (new_identity == NULL) {
		pr_err("out of memory\n");
		return -ENOMEM;
	}
	
	length = strlen(name);
	if (length > 20) {
		pr_err("length > 20\n");
		return -EINVAL;
	}
	
	strcpy(new_identity->name, name);
	new_identity->id = id;
	
	new_identity->busy = false;

	list_add(&new_identity->list, &identity_list);
	
	return ret;
}

struct identity *identity_find(int id)
{
	struct list_head *ptr;
	struct identity *entry;

	list_for_each(ptr, &identity_list) {
		entry = list_entry(ptr, struct identity, list);
		if (entry->id == id)
			return entry;
	}

	return NULL;		
}

void identity_destroy(int id)
{
	struct list_head *ptr, *q;
	struct identity *entry;

	list_for_each_safe(ptr, q, &identity_list) {
		entry = list_entry(ptr, struct identity, list);
		if (entry->id == id) {
			list_del(&entry->list);
			kmem_cache_free(snazy_cache, entry);
		}
	}
}

int init_module(void)
{
	int ret;
	struct identity *temp;

	snazy_cache = KMEM_CACHE(identity, 0);
	
	ret = identity_create("Alice", 1);
	if (ret != 0)
		goto create_out;
	
	ret = identity_create("Bob", 2);
	if (ret != 0)
		goto create_out;
	
	ret = identity_create("Dave", 3);
	if (ret != 0)
		goto create_out;
	
	ret = identity_create("Gena", 10);
	if (ret != 0)
		goto create_out;

	temp = identity_find(3);
	if (temp == NULL)
		goto find_out;

	temp = identity_find(42);
	if (temp == NULL)
		pr_info("id 42 not found\n");

	identity_destroy(2);
	identity_destroy(1);
	identity_destroy(10);
	identity_destroy(42);
	identity_destroy(3);

	return 0;
create_out:
	pr_err("create_out \n");
	return 0;
find_out:
	pr_err("find_out\n");
	return 0;
}

void cleanup_module(void)
{
	kmem_cache_destroy(snazy_cache);
	pr_debug("Goodbye World\n");
}
