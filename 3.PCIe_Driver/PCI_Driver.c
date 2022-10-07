/* Peripheral Component Interconnect (PCI) Driver
Theory @ https://static.lwn.net/images/pdf/LDD3/ch12.pdf
*/
/***************************************************************************************************************************************/
/* Includes */
#include<linux/module.h>    // Kernel header module (Kernel level header)
#include <linux/init.h>     // Init header
#include <linux/pci.h>      // Header that includes symbolic names for the PCI registers and several vendor and device ID values.
#include <linux/gpio.h>     // Header file for working with GPIOs

#define PCITTL32IO_VENDOR_ID 0x8086   // Vendor_Id (command: lspci -n and -v)
#define PCITTL32IO_DEVICE_ID 0x9c14   // Device_Id    

struct pcittl32io_gpiochip
{void __iomem *ptr_bar0;
 struct gpio_chip chip;
};

/***************************************************************************************************************************************/
/* ids,probe,remove*/

/* Create a ID list which lists all the devices which are compatible with our driver*/

static struct pci_device_id pcittl32io_ids[] =               // array pcittl32io_ids of type static struct
{{PCI_DEVICE(PCITTL32IO_VENDOR_ID, PCITTL32IO_DEVICE_ID) },  // This creates a struct pci_device_id that matches only the specific vendor and device ID.
 {}                                                          // Operating System knows list is complete when it finds {}
};

MODULE_DEVICE_TABLE(pci, pcittl32io_ids); //pci_device_id structure needs to be exported to user space (refer page : The PCI Interface | 311)

/*probe fun is called, when a PCI device is registered 
  @param 'dev' is pointer to the PCI device
  @param 'id' is pointer to the corresponding id table's entry
  @return '0' on success
  negative error code on failure
*/

static int pcittl32io_probe(struct pci_dev *dev, const struct pci_device_id *id) 
{u16 vid, did;
 u8 capability_ptr;
 u32 bar0, saved_bar0;
 printk("pcittl32io - Now I am in the probe function.\n");
 
 /* Let's read the PCIe VID and DID */
 
 if(0 != pci_read_config_word(dev, 0x0, &vid))              // access the config space
 {printk("pcittl32io - Error reading from config space\n"); // not success to read PCIe VID
  return -1;
 }
 printk("pcittl32io - VID; 0x%x\n", vid);                   // success to read PCIe VID 
 if(0 != pci_read_config_word(dev, 0x2, &did)) 
 {printk("pcittl32io - Error reading from config space\n"); // not success to read PCIe DID
  return -1;
 }
 printk("pcittl32io - DID; 0x%x\n", did);                   // success to read PCIe DID
 
 /* Read the pci capability pointer */
 
 printk("pcittl32io - VID; 0x%x\n", vid);
 if(0 != pci_read_config_byte(dev, 0x34, &capability_ptr)) 
 {printk("pcittl32io - Error reading from config space\n");
  return -1;
 }
 if(capability_ptr) printk("pcittl32io - PCI card has capabilities!\n");
 else printk("pcittl32io - PCI card doesn't have capabilities!\n");


 if(0 != pci_read_config_dword(dev, 0x10, &bar0)) 
 {printk("pcittl32io - Error reading from config space\n");
  return -1;
 }
 saved_bar0 = bar0;
 if(0 != pci_write_config_dword(dev, 0x10, 0xffffffff)) 
 {printk("pcittl32io - Error writing to config space\n");
  return -1;
 }
 if(0 != pci_read_config_dword(dev, 0x10, &bar0)) 
 {printk("pcittl32io - Error reading from config space\n");
  return -1;
 }
 if((bar0 & 0x3) == 1) printk("pcittl32io - BAR0 is IO space\n");
 else printk("pcittl32io - BAR0 is memory space\n");

 bar0 &= 0xFFFFFFFD; // clear the first two bist
 bar0 = ~bar0;       // invert
 bar0++;             // add 1 
 printk("pcittl32io - BAR0 is %d bytes big\n", bar0);
 
 if(0 != pci_write_config_dword(dev, 0x10, saved_bar0)) 
 {printk("pcittl32io - Error writing to config space\n");
  return -1;
 }
 return 0;
}


/* function pcittl32io_remove, this unction is called, when a PCI device is unregistered
  @parameter 'dev' pointer to the PCI device
*/
static void pcittl32io_remove(struct pci_dev *dev) 
{ struct pcittl32io_gpiochip * my_data = pci_get_drvdata(dev);
  printk("pcittl32io - Now I am in the remove function.\n");
  if(my_data) gpiochip_remove(&my_data->chip);
}

/***************************************************************************************************************************************/
/* PCI driver struct, it is structure that represents a PCI driver. All PCI drivers must define this.*/

static struct pci_driver pcittl32io_driver = 
{.name     = "pcittl32io",
 .id_table = pcittl32io_ids,
 .probe    = pcittl32io_probe,
 .remove   = pcittl32io_remove,
};

/***************************************************************************************************************************************/
/* initialization function, this function is called, when the module is loaded into the kernel */ 

static int __init my_init(void) 
{printk("pcittl32io - Registering the PCI device\n");
 return pci_register_driver(&pcittl32io_driver);
}

/***************************************************************************************************************************************/
/* Exit function, this function is called, when the module is loaded into the kernel */ 

static void __exit my_exit(void)
{printk("pcittl32io - Unregistering the PCI device\n");
 pci_unregister_driver(&pcittl32io_driver);
}

/***************************************************************************************************************************************/
/* registration of Modules */ 

module_init(my_init);    // module_init, add its parameter to init entry point of database of the kernel
module_exit(my_exit);    // module_exit, add its parameter to exit entry point of database of the kernel

/***************************************************************************************************************************************/
/*Module description */

MODULE_LICENSE ("GPL"); 			   // license type (GNU Public Licence)
MODULE_AUTHOR("Swapnil");    			   // Author of kernel module
MODULE_DESCRIPTION("PCIe driver"); 		   // Discription msg about kernel
//MODULE_INFO(board,"Board name"); // create own custom msg

