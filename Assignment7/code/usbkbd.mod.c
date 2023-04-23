#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
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
__used __section("__versions") = {
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x813c6f7b, "usb_kill_urb" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xceadc1c3, "usb_free_urb" },
	{ 0x66cbcbc1, "usb_free_coherent" },
	{ 0x37a0cba, "kfree" },
	{ 0x92997ed8, "_printk" },
	{ 0x5c65e38b, "input_unregister_device" },
	{ 0x2de8eb5f, "usb_submit_urb" },
	{ 0x34db050b, "_raw_spin_lock_irqsave" },
	{ 0xd35cce70, "_raw_spin_unlock_irqrestore" },
	{ 0xd3797fb9, "_dev_warn" },
	{ 0xa1933a44, "_dev_err" },
	{ 0x48f9fbf7, "input_event" },
	{ 0xf812cff6, "memscan" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0xd4263478, "_dev_info" },
	{ 0x54b1fac6, "__ubsan_handle_load_invalid_value" },
	{ 0xf301d0c, "kmalloc_caches" },
	{ 0x35789eee, "kmem_cache_alloc_trace" },
	{ 0x6e5e3851, "input_allocate_device" },
	{ 0x3c913dfb, "usb_alloc_urb" },
	{ 0xfa11f84e, "usb_alloc_coherent" },
	{ 0x754d539c, "strlen" },
	{ 0xf9c0b663, "strlcat" },
	{ 0xa916b694, "strnlen" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x3a26ba6b, "input_register_device" },
	{ 0x69d7b3d0, "device_set_wakeup_enable" },
	{ 0x5994c673, "input_free_device" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0xaca7784b, "usb_register_driver" },
	{ 0x6d7d5d36, "usb_deregister" },
	{ 0x541a6db8, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("usb:v*p*d*dc*dsc*dp*ic03isc01ip01in*");

MODULE_INFO(srcversion, "2E39ECDFD7F05CF2D15BC68");
