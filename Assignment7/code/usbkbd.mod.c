#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
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
	{ 0x32e21920, "module_layout" },
	{ 0xdf85ea06, "usb_deregister" },
	{ 0xf63cc4cc, "usb_register_driver" },
	{ 0x35b2d065, "input_free_device" },
	{ 0xc30d46b2, "device_set_wakeup_enable" },
	{ 0xb668d3fc, "input_register_device" },
	{ 0x656e4a6e, "snprintf" },
	{ 0xf9c0b663, "strlcat" },
	{ 0x754d539c, "strlen" },
	{ 0x6f4cc9ff, "usb_alloc_coherent" },
	{ 0x8a7330c7, "usb_alloc_urb" },
	{ 0x710c51f5, "input_allocate_device" },
	{ 0xf35141b2, "kmem_cache_alloc_trace" },
	{ 0x26087692, "kmalloc_caches" },
	{ 0x54b1fac6, "__ubsan_handle_load_invalid_value" },
	{ 0x92997ed8, "_printk" },
	{ 0xe6e002cf, "_dev_info" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0xf812cff6, "memscan" },
	{ 0x659ed02c, "input_event" },
	{ 0x79161eb9, "_dev_err" },
	{ 0x400d1b40, "_dev_warn" },
	{ 0xd35cce70, "_raw_spin_unlock_irqrestore" },
	{ 0x34db050b, "_raw_spin_lock_irqsave" },
	{ 0xe06e0338, "usb_submit_urb" },
	{ 0x3696b2c2, "input_unregister_device" },
	{ 0x37a0cba, "kfree" },
	{ 0x7212d1e6, "usb_free_coherent" },
	{ 0xb8b75f24, "usb_free_urb" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x2da2081e, "usb_kill_urb" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("usb:v*p*d*dc*dsc*dp*ic03isc01ip01in*");

MODULE_INFO(srcversion, "3D676AFA6CF1A0DE050986E");
