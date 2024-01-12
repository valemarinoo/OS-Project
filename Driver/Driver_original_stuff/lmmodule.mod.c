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
	{ 0x92997ed8, "_printk" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xfb578fc5, "memset" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0xc1352057, "__register_chrdev" },
	{ 0xaee657ee, "__class_create" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x3818fa5c, "device_create" },
	{ 0x645620c0, "class_destroy" },
	{ 0xeabef977, "device_destroy" },
	{ 0x7377b2e, "class_unregister" },
	{ 0x541a6db8, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "7E630BFBC1F44FB21F2BBA2");


**License: [CC BY-NC 4.0](https://creativecommons.org/licenses/by-nc/4.0/)**