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
 *
 I think I will use depmod to resolve module dependancies
 * udev(replaced /sbin/hotplug) to load kernel module
 *
 * Events can be injected (think simulating keyboard hotplug) into the hotplug
 * mechanism through sysfs via the "uevent" files
 *
 * usb_device_id - if I want my driver (module) to be called  for every USB
 * device in the system, create a entry that sets only the driver_info field
 */

static int usb_snazy_probe(struct usb_interface *iface,
                           const struct usb_device_id *id)
{
        printk(KERN_INFO "snazy_usb module probe");
        return 0;
}

static void usb_snazy_disconnect(struct usb_interface *intf)
{
        printk(KERN_INFO "snazy_usb module disconnect");
}

static struct usb_device_id usb_snazy_id_table [] = {
        //        {USB_DEVICE_INTERFACE_CLASS(LOGI_KBD_VENDOR,LOGI_KBD_PROD, USB_INTERFACE_PROTOCOL_KEYBOARD)},
        //{.driver_info = 42},
        {USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT,USB_INTERFACE_PROTOCOL_KEYBOARD)},
        {},
};
MODULE_DEVICE_TABLE(usb, usb_snazy_id_table);

static struct usb_driver usb_snazy_driver = {
        .name = "usb_snazy",
        .probe = usb_snazy_probe,
        .disconnect = usb_snazy_disconnect,
        .id_table = usb_snazy_id_table,
};
/*
static int __init usb_snazy_init(void)
{
        int result;
        printk(KERN_DEBUG "Hello world!\n");

        result = usb_register(&usb_snazy_driver);
        if(result)
                printk(KERN_ERR "usb_register_failed. Err number %d", result);

        return result;
}
module_init(usb_snazy_init);

static void __exit usb_snazy_exit(void)
{
        printk(KERN_DEBUG "Goodbye World\n");
        usb_deregister(&usb_snazy_driver);
}
module_exit(usb_snazy_exit);
*/

module_usb_driver(usb_snazy_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tyler Olivieri - Snazyman");
MODULE_DESCRIPTION("Eudyptula challenge task 5");
