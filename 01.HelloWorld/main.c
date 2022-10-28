/* Dynamically Loadable Linux Kernel Module */

/***************************************************************************************************************************************/
/* Includes */

#include<linux/module.h>    // Kernel header module (Kernel level header)
                            // Kernel Header file location: LINUX_SRC/include/linux
                            // most of the relevent kernel headers are at: linux_source_bse/include/linux/
                            // Path of header: /lib/modules/5.15.0-48-generic/build/include/linux
                            // never include user header module l.e. stdio.h etc.
/***************************************************************************************************************************************/
/* initialization */                            
                            
/*This is module initialization entry point (like main)*/  
/*fun Prototype: int fun_name(void) */   
/*In case of static modules,this function will be called during boot time */  
/*In case of dynamic modules,this function will get called during module insertion */  
                      
static int __init helloworld_init(void) // it make sense to make this function private using 'static' though it is optional
                                        // __init is a macro. Function selection attributes
                                        // __init indicates compiler directive, which directs the compiler to keep data or code in an output
                                        // section called  ".init" in the final elf kernel image
{
 pr_info("Hello world\n"); // kernel's print function
 return 0;                 // must return a value. 0=Success, non zero means module initialization failed
}

/***************************************************************************************************************************************/
/* clean-up */  

/*This is module clean-up entry point*/  
/*fun Prototype: void fun_name(void) */   
/*This is entry point when the module is removed*/  
/*Since we cannot remove static modules,cleanup function will be get called only in case of dynamic modules */  
/*dynamic modules, removed using user space command such as 'rmmode' */  
/*Even if your static module has a clean-up function, kernel build system will remove it during build process
if there is an '__exit' marker/macro*/
/* __exit indicates compiler directive, which directs the compiler to keep data or code in an output
section called  ".exit" in the final kernel image */  
/* using __init with function, kernel will free the code memory of that function after its execution*/  
/* using __initdata with variables, kernel will free the memory of that variables after its execution*/  
/* __init and __exit makes sense only for static modules (built-in modules)*/  

static void __exit helloworld_cleanup(void)
{
 pr_info("Good bye world\n"); // undoing init function. 
                              // free memory which are requested in the init function
}

/***************************************************************************************************************************************/
/* registration of Modules */ 

/*This is registration of above modules entry points with kernerl */
/*module_init and module_exit are not function, but macro defined in linux/module.h */
module_init(helloworld_init);    // module_init, add its parameter to init entry point of database of the kernel
module_exit(helloworld_cleanup); // module_exit, add its parameter to exit entry point of database of the kernel

/***************************************************************************************************************************************/
/*Module description */
MODULE_LICENSE ("GPL"); // license type (GNU Public Licence)
MODULE_AUTHOR("ME");    // Athor of kernel module
MODULE_DESCRIPTION("A Simple hellop world kenal module"); // Discription msg about kernel
MODULE_INFO(board,"Board name"); // create own custom msg

/***************************************************************************************************************************************/

/*compile linux driver code*/
/* make -C /lib/modules/5.15.0-48-generic/build/ M=$PWD modules */
/* nm main.ko  */
/* vim main.mod.c  */



























