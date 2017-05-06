#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/hid.h>

/*
 * When USB keyboard is plugged in, the module will be automatically loaded
 * by correct USB hotplug tools
 * depmod/kmod/udev/mdev/systemd
 */

static struct usb_device_id usb_snazy_id_table[] = {
	{USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID, \
			    USB_INTERFACE_SUBCLASS_BOOT, \
			    USB_INTERFACE_PROTOCOL_KEYBOARD)},
	{},
};
MODULE_DEVICE_TABLE(usb, usb_snazy_id_table);


int init_module(void)
{
	pr_debug("Hello World!\n");
	return 0;
}

void cleanup_module(void)
{
	pr_debug("Goodbye World\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tyler Olivieri - Snazyman");
MODULE_DESCRIPTION("Eudyptula challenge task 5");
