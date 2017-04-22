#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/hid.h>

#define LOGI_KBD_VENDOR 0x046D
#define LOGI_KBD_PROD  0xC31C

/*
 * When USB keyboard is plugged in, the module will be automatically loaded
 * by correct USB hotplug tools
 * depmod/kmod/udev/mdev/systemd
 */

static int usb_snazy_probe(struct usb_interface *iface,
			   const struct usb_device_id *id)
{
	pr_info("snazy_usb module probe\n");
	return 0;
}

static void usb_snazy_disconnect(struct usb_interface *intf)
{
	pr_info("snazy_usb module disconnect\n");
}

static struct usb_device_id usb_snazy_id_table[] = {
	{USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID, \
			    USB_INTERFACE_SUBCLASS_BOOT, \
			    USB_INTERFACE_PROTOCOL_KEYBOARD)},
	{},
};
MODULE_DEVICE_TABLE(usb, usb_snazy_id_table);

static struct usb_driver usb_snazy_driver = {
	.name = "usb_snazy",
	.probe = usb_snazy_probe,
	.disconnect = usb_snazy_disconnect,
	.id_table = usb_snazy_id_table,
};

module_usb_driver(usb_snazy_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tyler Olivieri - Snazyman");
MODULE_DESCRIPTION("Eudyptula challenge task 5");
