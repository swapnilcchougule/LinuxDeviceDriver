/* Dynamically Loadable, Psudo char driver Linux Kernel Module */

/***************************************************************************************************************************************/
/* Includes */
#include<linux/module.h>    // Kernel header module (Kernel level header)
#include<linux/fs.h>        // file systems (alloc_chrdev_region(),unregister_chrdev_region())
#include<linux/cdev.h>      // char driver (cdev_init(),cdev_add,cdev_del())

#include<linux/device.h>
#include<linux/kdev_t.h>
#include<linux/uaccess.h>

/***************************************************************************************************************************************/
/* Variables */

#define DEV_MEM_SIZE 512
char device_buffer[DEV_MEM_SIZE]; // Psudo device memory
dev_t device_number; 		   // thsis variable holds device number

struct cdev pcd_cdev; 		   // cdev variable (cdev= character driver)
struct class *class_pcd;   	   // holds the class pointer 
struct device *device_pcd;        // holds the device pointer 


/***************************************************************************************************************************************/
/*sysfs function defination*/

/*char driver open function*/
int pcd_open(struct inode *inode, struct file *filp)
{pr_info("open was successful\n");
 printk(KERN_INFO "Device File Opened...!!!\n");
 return 0;
}

/*char driver release function*/
int pcd_release(struct inode *inode, struct file *flip)
{pr_info("release was successful\n");
 printk(KERN_INFO "Device File Closed...!!!\n");
 return 0;
}
 
/*char driver read function*/
ssize_t pcd_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{pr_info("Read requested for %zu bytes \n",count);
 pr_info("Current file position = %lld\n",*f_pos);
 /* Adjust the 'count' */
 if((*f_pos + count) > DEV_MEM_SIZE) count = DEV_MEM_SIZE - *f_pos;
 /*copy to user */
 if(copy_to_user(buff,&device_buffer[*f_pos],count))
 {return -EFAULT;
 }
 /*update the current file postion */
 *f_pos += count;
 pr_info("Number of bytes successfully read = %zu\n",count);
 pr_info("Updated file position = %lld\n",*f_pos);
 /*Return number of bytes which have been successfully read */
 return count;
} 

/*char driver write function*/
ssize_t pcd_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{pr_info("Write requested for %zu bytes\n",count);
 pr_info("Current file position = %lld\n",*f_pos);
 /* Adjust the 'count' */
 if((*f_pos + count) > DEV_MEM_SIZE) count = DEV_MEM_SIZE - *f_pos;
 if(!count)
 {pr_err("No space left on the device \n");
  return -ENOMEM;
 }
 /*copy from user */
 if(copy_from_user(&device_buffer[*f_pos],buff,count))
 {return -EFAULT;
 }
 /*update the current file postion */
 *f_pos += count;
 pr_info("Number of bytes successfully written = %zu\n",count);
 pr_info("Updated file position = %lld\n",*f_pos);
 /*Return number of bytes which have been successfully written */
 return count;
}  

/*char driver lseek function*/
loff_t pcd_lseek(struct file *filp, loff_t offset, int whence)
{loff_t temp;
 pr_info("lseek requested \n");
 pr_info("Current value of the file position = %lld\n",filp->f_pos);
 switch(whence)
 {		
 	case SEEK_SET:
		if((offset > DEV_MEM_SIZE) || (offset < 0))
		return -EINVAL;
		filp->f_pos = offset;
		break;
	case SEEK_CUR:
		temp = filp->f_pos + offset;
		if((temp > DEV_MEM_SIZE) || (temp < 0))
		return -EINVAL;
		filp->f_pos = temp;
		break;
	case SEEK_END:
		temp = DEV_MEM_SIZE + offset;
		if((temp > DEV_MEM_SIZE) || (temp < 0))
		return -EINVAL;
		filp->f_pos = temp;
		break;
	default:
		return -EINVAL;
 }
 pr_info("New value of the file position = %lld\n",filp->f_pos);
 return filp->f_pos;
}

/* file operations of the driver */
struct file_operations pcd_fops=
{.open    = pcd_open,
 .release = pcd_release,
 .read    = pcd_read,
 .write   = pcd_write,
 .owner   = THIS_MODULE
};

/***************************************************************************************************************************************/
/* initialization */                            
                                               
static int __init pcd_driver_init(void) // 
{ 
  int ret;
  
 /*1. Allocating Major number dynamically*/
 ret = alloc_chrdev_region(&device_number,0,1,"PusdoCharDriver_Swapnil");  //Create device number and dynamically allocate a device number 
 if(ret < 0)
  {pr_err("Alloc chrdev failed\n");
   printk(KERN_INFO "Cannot allocate major number\n");
   goto out;
  }
 printk(KERN_INFO "Major = %d and Minor = %d \n",MAJOR(device_number), MINOR(device_number));
 pr_info("Device number <major>:<minor> = %d:%d\n",MAJOR(device_number),MINOR(device_number));
 
 /*2. Creating cdev structure*/ 
 cdev_init(&pcd_cdev,&pcd_fops); 		   //Initialize the cdev structure with fops (file operations)   

 /*3. Adding character device to the VFS system*/
 pcd_cdev.owner = THIS_MODULE;
 ret = cdev_add(&pcd_cdev,device_number,1);      //Register a device (cdev structure) with VFS   
 if(ret < 0)
  {pr_err("Cdev add failed\n");
   printk(KERN_INFO "Cannot add the device to the system\n");
   goto unreg_chrdev;
  }
  
 /*4. Creating struct class*/
 class_pcd = class_create(THIS_MODULE,"pcd_class"); //create device class under /sys/class/ 
 if(IS_ERR(class_pcd))
 {pr_err("Class creation failed\n");
  ret = PTR_ERR(class_pcd);
  goto cdev_del;
 }
 
 /*5. Creating device*/
 device_pcd = device_create(class_pcd,NULL,device_number,NULL,"pcd"); //populate the sysfs with device information
                                                                      // created file appears at '/dev/' directory               
 if(IS_ERR(device_pcd))
 {pr_err("Device create failed\n");
  ret = PTR_ERR(device_pcd);
  goto class_del;
 }
 
 printk(KERN_INFO "Device Driver Insert Done Properly...!!!\n");
 return 0;                 // must return a value. 0=Success, non zero means module initialization failed

class_del:
	class_destroy(class_pcd); 
cdev_del:
	cdev_del(&pcd_cdev);	 
unreg_chrdev:
	unregister_chrdev_region(device_number,1);       
	//cdev_del(&pcd_cdev);
	//return -1;
out:
	pr_info("Module insertion failed\n");
	return ret;
}

/***************************************************************************************************************************************/
/* clean-up */  

static void __exit pcd_driver_cleanup(void)
{
 device_destroy(class_pcd,device_number);
 class_destroy(class_pcd);
 cdev_del(&pcd_cdev);
 unregister_chrdev_region(device_number,1);
 pr_info("Char device driver module unloaded\n");
}

/***************************************************************************************************************************************/
/* registration of Modules */ 

module_init(pcd_driver_init);    // module_init, add its parameter to init entry point of database of the kernel
module_exit(pcd_driver_cleanup); // module_exit, add its parameter to exit entry point of database of the kernel

/***************************************************************************************************************************************/
/*Module description */

MODULE_LICENSE ("GPL"); 			   // license type (GNU Public Licence)
MODULE_AUTHOR("Swapnil");    			   // Author of kernel module
MODULE_DESCRIPTION("A pseudo character driver"); // Discription msg about kernel
//MODULE_INFO(board,"Board name"); // create own custom msg

/***************************************************************************************************************************************/

/*compile linux driver code*/
/* make -C /lib/modules/5.15.0-48-generic/build/ M=$PWD modules */
/* make and make clean*/




























