/* Implementing a Char Driver */
/* 
Step 1: Allocate the device number (major/minor). This can be done with register_chrdev_region() or alloc_chrdev_region().
Step 2: Implement the file operation (open, read, write, ioctl, etc).
Step 3: Register the char driver in the kernel with cdev_init() and cdev_add() .
*/
/***************************************************************************************************************************************/
/* Includes */
#include<linux/module.h>    // Kernel header module (Kernel level header)
#include<linux/fs.h>        // file systems (alloc_chrdev_region(),unregister_chrdev_region())
#include<linux/cdev.h>      // char driver (cdev_init(),cdev_add,cdev_del())
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define DRIVER_NAME "drvled"  // define driver name
#define LED_OFF 	0
#define LED_ON		1

static struct
{dev_t devnum;             // var to store device number(Major and Minor)
 struct cdev cdev;         // cdev struct to register the char driver in the kernel
 unsigned int led_status;   
} drvled_data;

static void devled_setled(unsigned int status)
{drvled_data.led_status = status;
}

/***************************************************************************************************************************************/
/* drvled_read function */

static ssize_t drvled_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) 
{static const char* const msg[]={"OFF\n", "ON\n"};
 int size;
 if (*ppos>0) return 0;  			  // check if EOF 
 size = strlen(msg[drvled_data.led_status]);
 if (size > count) size = count;
 if(copy_to_user(buf,msg[drvled_data.led_status],size)) return -EFAULT;
 *ppos +=size;
 return size;
}

static ssize_t drvled_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) 
{char kbuf=0;
 if (copy_from_user(&kbuf,buf,1)) return -EFAULT;
 if(kbuf =='1')
  {devled_setled(LED_ON);
   pr_info("LED ON!\n");
  }
 else if(kbuf =='0')
  {devled_setled(LED_OFF);
   pr_info("LED OFF!\n");
  }
 return count;
}

/***************************************************************************************************************************************/
/* file operations of the driver */

static struct file_operations drvled_fops = 
{.owner = THIS_MODULE,
 .read = drvled_read,          
 .write = drvled_write,
 //.open = driver_open,
 //.release = driver_close,
};

/***************************************************************************************************************************************/
/* This function is called, when the module is loaded into the kernel */

static int __init drvled_init(void) 
{int result;
 printk("Hello, Kernel!\n");
 
 /*1. Allocate the device number dynamically*/
 result = alloc_chrdev_region(&drvled_data.devnum, 0, 1, DRIVER_NAME);
 if(result) 
 {printk("Device Nr. could not be allocated!\n");
  return result;
 }
 pr_info("Device number <major>:<minor> = %d:%d\n",MAJOR(drvled_data.devnum),MINOR(drvled_data.devnum));
 
 /*2. Register the char driver in the kernel */
 cdev_init(&drvled_data.cdev, &drvled_fops);  		 // Initialize device file 
 
 result = cdev_add(&drvled_data.cdev, drvled_data.devnum, 1);  // Regisering device to kernel 
 if (result) 
 {printk("Registering of Char device to kernel failed!\n");
  unregister_chrdev_region(drvled_data.devnum, 1);
  return result;
 }
 
 devled_setled(LED_OFF);
 pr_info("%s : Initalized \n",DRIVER_NAME);
 return 0;
}

/***************************************************************************************************************************************/
/* This function is called, when the module is removed from the kernel */

static void __exit drvled_exit(void) 
{cdev_del(&drvled_data.cdev);
 unregister_chrdev_region(drvled_data.devnum, 1);
 printk("Goodbye, Kernel\n");
 pr_info("%s : exiting \n",DRIVER_NAME);
}

/***************************************************************************************************************************************/
/* registration of Modules */ 
/*This is registration of above modules entry points with kernerl */
/*module_init and module_exit are not function, but macro defined in linux/module.h */

module_init(drvled_init);    // module_init, add its parameter to init entry point of database of the kernel
module_exit(drvled_exit);    // module_exit, add its parameter to exit entry point of database of the kernel

/***************************************************************************************************************************************/
/*Module description */

MODULE_LICENSE ("GPL"); 				    // license type (GNU Public Licence)
MODULE_AUTHOR("Swapnil");   				    // Athor of kernel module
MODULE_DESCRIPTION("Implementing a Char Driver and Understanding structure of Linux Kernel Device Driver"); // Discription msg about kernel
MODULE_INFO(board,"Board name"); 			    // create own custom msg




