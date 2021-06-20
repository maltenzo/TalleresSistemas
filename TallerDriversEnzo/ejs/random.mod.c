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
	{ 0xc7b3657, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x37b77b0c, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0xb678368a, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0xa6f25ce9, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x99521d1c, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x9100996f, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0xf10c72d4, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0x8a1d0c32, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x50eedeb8, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x2276db98, __VMLINUX_SYMBOL_STR(kstrtoint) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x12da5bb2, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x4f8b5ddb, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0xb81960ca, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0x79aa04a2, __VMLINUX_SYMBOL_STR(get_random_bytes) },
	{ 0x15976daf, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xc73fcc54, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xb4390f9a, __VMLINUX_SYMBOL_STR(mcount) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "3AB8C1A7D864489625C2C1B");
