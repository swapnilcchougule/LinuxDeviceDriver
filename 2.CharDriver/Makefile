# This is the local make file, we should define a kbuild variable as below
# obj-<X> := <module_name>.o     
# obj-<X> is kbuild variable and 'X' value n/y/m
# n : Do not compile the module
# y : compile the module and link with kernel image
# n : compile as dynamically lodable kernel module  
# kbuild system will build main.o from main.c and after linking, will result in kernel module main.ko   
# uname -r = 5.15.0-50-generic

obj-m := CharDriver.o

KDIR = /lib/modules/5.15.0-50-generic/build/

all:
	make -C $(KDIR) M=$(PWD) modules
install:
	make -C $(KDIR) M=$(PWD) module_install
clean:
	make -C $(KDIR) M=$(PWD) clean
help:
	make -C $(KDIR) M=$(PWD) help	


# make -C <path to linux kernel tree> M= <path to your module> [Tagrget] 
# make -C $ KDIR M=$ PWD [Tagrget] 
# Target : modules, modules_install, clean, help
# make -C /lib/modules/5.15.0-48-generic/build/ M=$PWD modules
# sudo insmod main.ko   // insert the module in running kernel
# sudo rmmod main.ko    // remove the module in running kernel
# dmesg 		 // see the result 
# lsmod | head -5
# cat /proc/devices     // check the device and major number

# cc main.c -o main     // creat application executable 
