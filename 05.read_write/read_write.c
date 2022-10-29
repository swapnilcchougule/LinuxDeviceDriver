/* Dynamically Loadable, Device Number Linux Kernel Module */
/***************************************************************************************************************************************/
/* Includes */
#include<linux/module.h>    // Kernel header module (Kernel level header)
#include<linux/fs.h>        // file systems (alloc_chrdev_region(),unregister_chrdev_region())
#include<linux/cdev.h>      // char driver (cdev_init(),cdev_add,cdev_del())
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define DRIVER_NAME "dummydriver"
#define DRIVER_CLASS "MyModuleClass"
#define minor_nr 1                          // define minor number

/***************************************************************************************************************************************/
/* Buffer for data */
static char buffer[255];
static int buffer_pointer = 0;
/* Variables for device and device class */
static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;


/***************************************************************************************************************************************/
/* Read data out of the buffer  */

static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs) 
{int to_copy, not_copied, delta;
 to_copy = min(count, buffer_pointer);                    // Get amount of data to copy 
 not_copied = copy_to_user(user_buffer, buffer, to_copy); // Copy data to user , returns amount of bytes it hasn't copied 
 delta = to_copy - not_copied; 	                   // Calculate data 
 return delta;           				    // return amount of data read out of buffer
}

/***************************************************************************************************************************************/
/* Write data to buffer  */

static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) 
{int to_copy, not_copied, delta;
 to_copy = min(count, sizeof(buffer)); 		      // Get amount of data to copy 
 not_copied = copy_from_user(buffer, user_buffer, to_copy); // Copy data to user (destination,source,to copy)
 buffer_pointer = to_copy; 				      // Calculate data 
 delta = to_copy - not_copied;
 return delta;
}

/***************************************************************************************************************************************/
/* This function is called, when the device file is opened */

static int driver_open(struct inode *device_file, struct file *instance) 
{printk("dev_nr - open was called!\n");
 return 0;
}

/***************************************************************************************************************************************/
/* This function is called, when the device file is closed */

static int driver_close(struct inode *device_file, struct file *instance) 
{printk("dev_nr - close was called!\n");
 return 0;
}

/***************************************************************************************************************************************/
/* file operations of the driver */
static struct file_operations fops = 
{.owner = THIS_MODULE,
 .open = driver_open,
 .release = driver_close,
 .read = driver_read,
 .write = driver_write
};

/***************************************************************************************************************************************/
/* This function is called, when the module is loaded into the kernel */

static int __init ModuleInit(void) 
{int retval;
 printk("Hello, Kernel!\n");
 
 /*1. Allocating Major device number dynamically*/
 if( alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0) 
 {printk("Device Nr. could not be allocated!\n");
  return -1;
 }
 printk("read_write - Device Nr. Major: %d, Minor: %d was registered!\n", my_device_nr >> 20, my_device_nr && 0xfffff);
// pr_info("Device number <major>:<minor> = %d:%d\n",MAJOR(my_device_nr),MINOR(my_device_nr));
 
 /* Create device class */
 if((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) 
 {printk("Device class can not be created!\n");
  goto ClassError;
 }

 /* create device file */
 if(device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL) 
 {printk("Can not create device file!\n");
  goto FileError;
 }

 /* Initialize device file */
 cdev_init(&my_device, &fops);

 /* Regisering device to kernel */
 if(cdev_add(&my_device, my_device_nr, 1) == -1) 
 {printk("Registering of device to kernel failed!\n");
  goto AddError;
 }
 
 return 0;
 
AddError:
	device_destroy(my_class, my_device_nr);
FileError:
	class_destroy(my_class);
ClassError:
	unregister_chrdev_region(my_device_nr, 1);
	return -1;
}

/***************************************************************************************************************************************/
/* This function is called, when the module is removed from the kernel */
static void __exit ModuleExit(void) 
{cdev_del(&my_device);
 device_destroy(my_class, my_device_nr);
 class_destroy(my_class);
 unregister_chrdev_region(my_device_nr, 1);
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











