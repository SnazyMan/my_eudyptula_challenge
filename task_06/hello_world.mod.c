#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x66594595, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xd26cf92a, __VMLINUX_SYMBOL_STR(misc_deregister) },
	{ 0x4e1f3210, __VMLINUX_SYMBOL_STR(misc_register) },
	{ 0x619cb7dd, __VMLINUX_SYMBOL_STR(simple_read_from_buffer) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0xe2d5255a, __VMLINUX_SYMBOL_STR(strcmp) },
	{ 0xbb4f4766, __VMLINUX_SYMBOL_STR(simple_write_to_buffer) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "E422AE622D865F64313530F");
