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
	{ 0xe49bb82b, "module_layout" },
	{ 0xc9b822eb, "pci_unregister_driver" },
	{ 0x9c75f2bc, "__pci_register_driver" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0xe5ac9a30, "pci_write_config_dword" },
	{ 0xe6edc564, "pci_read_config_dword" },
	{ 0xda79796f, "pci_read_config_byte" },
	{ 0xce7ad4e4, "pci_read_config_word" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x91f60a19, "gpiochip_remove" },
	{ 0x92997ed8, "_printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("pci:v00008086d00009C14sv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "9DBE39F7DAB0911CA6E451F");
