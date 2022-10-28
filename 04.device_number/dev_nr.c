/* Dynamically Loadable, Device Number Linux Kernel Module */
/***************************************************************************************************************************************/
/* Includes */
#include<linux/module.h>    // Kernel header module (Kernel level header)
#include<linux/fs.h>        // file systems (alloc_chrdev_region(),unregister_chrdev_region())
#include<linux/cdev.h>      // char driver (cdev_init(),cdev_add,cdev_del())

/***************************************************************************************************************************************/
/* This function is called, when the device file is opened */

static int driver_open(struct inode *device_file, struct file *instance) 
{printk("dev_nr - open was called!\n");
 return 0;
}

/***************************************************************************************************************************************/
/* This function is called, when the device file is opened */

static int driver_close(struct inode *device_file, struct file *instance) 
{printk("dev_nr - close was called!\n");
 return 0;
}

/***************************************************************************************************************************************/
/* file operations of the driver */

static struct file_operations fops = 
{.owner = THIS_MODULE,
 .open = driver_open,
 .release = driver_close
};

#define MYMAJOR 64     // define Major numbar

/***************************************************************************************************************************************/
/* This function is called, when the module is loaded into the kernel */

static int __init ModuleInit(void) 
{int retval;                            
 printk("Hello, Kernel!\n");
 
/* register device nr. */
 retval = register_chrdev(MYMAJOR, "my_dev_nr", &fops);
 if(retval == 0) 
 {printk("dev_nr - registered Device number Major: %d, Minor: %d\n", MYMAJOR, 0);
 }
 else if(retval > 0)
 {printk("dev_nr - registered Device number Major: %d, Minor: %d\n", retval>>20, retval&0xfffff);
 }
 else
 {printk("Could not register device number!\n");
  return -1;
  }
 return 0;
}

/***************************************************************************************************************************************/
/* This function is called, when the module is removed from the kernel */
 
static void __exit ModuleExit(void) 
{unregister_chrdev(MYMAJOR, "my_dev_nr");
 printk("Goodbye, Kernel\n");
}


/***************************************************************************************************************************************/
/* registration of Modules */ 

/*This is registration of above modules entry points with kernerl */
/*module_init and module_exit are not function, but macro defined in linux/module.h */
module_init(ModuleInit);    // module_init, add its parameter to init entry point of database of the kernel
module_exit(ModuleExit);    // module_exit, add its parameter to exit entry point of database of the kernel

/***************************************************************************************************************************************/
/*Module description */
MODULE_LICENSE ("GPL"); 				    // license type (GNU Public Licence)
MODULE_AUTHOR("Swapnil");   				    // Athor of kernel module
MODULE_DESCRIPTION("Registers a device number and implement some callback functions"); // Discription msg about kernel
MODULE_INFO(board,"Board name"); 			    // create own custom msg











