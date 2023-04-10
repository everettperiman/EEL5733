#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x7ef2b274, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xecaaca53, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x942cfb36, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0xd3d9c965, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0xc0c52091, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x2aeb6dc7, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x76a81688, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x32989da9, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xef4c5632, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0xd8e484f0, __VMLINUX_SYMBOL_STR(register_chrdev_region) },
	{ 0xd2b09ce5, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0xb44ad4b3, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0xa202a8e5, __VMLINUX_SYMBOL_STR(kmalloc_order_trace) },
	{ 0x754d539c, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0xfb578fc5, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x7c2d098f, __VMLINUX_SYMBOL_STR(krealloc) },
	{ 0x362ef408, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x88db9f48, __VMLINUX_SYMBOL_STR(__check_object_size) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "B4F26BFCE2844B1D471362C");
