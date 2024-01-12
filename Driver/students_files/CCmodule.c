/* 
 * @file   module.c
 * @author Matteo Masciocchi
 *
 * sudo apt-get install linux-headers-$(uname -r) build-essential
 */
 
#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <linux/uaccess.h>          // Required for the copy to user function
#include <linux/string.h>
#include <linux/io.h>
#include "CCmodule.h"


MODULE_LICENSE("GPL");            ///< The license type -- this affects available functionality
MODULE_AUTHOR("Matteo Masciocchi");    ///< The author -- visible when you use modinfo
MODULE_DESCRIPTION("An amazing driver for our CryptoCore");  ///< The description -- see modinfo
MODULE_VERSION("1.0");            ///< A version number to inform users
 
static int    majorNumber;                  ///< Stores the device number -- determined automatically
static struct class*  CCcharClass  = NULL; ///< The device-driver class struct pointer
static struct device* CCcharDevice = NULL; ///< The device-driver device struct pointer

 
// The prototype functions for the character driver -- must come before the struct definition
static ssize_t CC_encrypt(const char *, char *, size_t);
static ssize_t CC_decrypt(const char *, char *, size_t);
static long int CC_ioctl(struct file *, unsigned int , unsigned long ); 

/** @brief Devices are represented as file structure in the kernel. The file_operations structure from
 *  /linux/fs.h lists the callback functions that you wish to associated with your file operations
 *  using a C99 syntax structure. char devices usually implement open, read, write and release calls
 */
static struct file_operations fops =
{
   .owner = THIS_MODULE,
   .unlocked_ioctl = CC_ioctl
};

struct CC_data {
	//put your data structure here
};

static long int CC_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
	struct CC_data data;
	
	if(copy_from_user(&data, (struct CC_data *)arg, sizeof(data)))
		return -1;
	
	switch(cmd){
		case ENCRYPT:
			CC_encrypt(data.input, data.output, data.len);
			break;
		case DECRYPT:
			CC_decrypt(data.input, data.output, data.len);
			break;
		default:
			return -2;
	}
	
	return 0;

}


/** @brief This function is responisble for decryption.
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 *  @param input The pointer to the buffer contianing the input data
 *  @param output The pointer to the buffer contianing the decrypted data
 *  @param len Length of the string to be encrypted
 */
static ssize_t CC_decrypt(const int * input, int * output, size_t len) {
   	printk(KERN_INFO "CC: executing decrypt\n");
	
	//write the input to the right memory addresses
	//start the conversion
	//read the outputs from the right memory addresses

	return 0;
}


/** @brief This function is responisble for ecnryption.
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 *  @param input The pointer to the buffer contianing the input data
 *  @param output The pointer to the buffer contianing the encrypted data
 *  @param len Length of the string to be encrypted
 */
static ssize_t CC_encrypt(const int * input, int * output, size_t len) {
   	printk(KERN_INFO "CC: executing encrypt\n");
	
	//write the input to the right memory addresses
	//start the conversion
	//read the outputs from the right memory addresses

	return 0;
}


/** @brief The LKM initialization function
 *  The static keyword restricts the visibility of the function to within this C file. The __init
 *  macro means that for a built-in driver (not a LKM) the function is only used at initialization
 *  time and that it can be discarded and its memory freed up after that point.
 *  @return returns 0 if successful
 */
static int __init CC_module_init(void){
   printk(KERN_INFO "CC: Initializing the CryptoCore driver\n");
 
   // Try to dynamically allocate a major number for the device -- more difficult but worth it
   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   if (majorNumber<0){
      printk(KERN_ALERT "CC failed to register a major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "CC: registered correctly with major number %d\n", majorNumber);
 
   // Register the device class
   CCcharClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(CCcharClass)){                // Check for error and clean up if there is
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(CCcharClass);          // Correct way to return an error on a pointer
   }
   printk(KERN_INFO "CC: device class registered correctly\n");
 
   // Register the device driver
   CCcharDevice = device_create(CCcharClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(CCcharDevice)){               // Clean up if there is an error
      class_destroy(CCcharClass);           // Repeated code but the alternative is goto statements
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(CCcharDevice);
   }
   printk(KERN_INFO "CC: device class created correctly\n"); // Made it! device was initialized
   return 0;
}
 
/** @brief The LKM cleanup function
 *  Similar to the initialization function, it is static. The __exit macro notifies that if this
 *  code is used for a built-in driver (not a LKM) that this function is not required.
 */
static void __exit CC_module_exit(void){
   device_destroy(CCcharClass, MKDEV(majorNumber, 0));     // remove the device
   class_unregister(CCcharClass);                          // unregister the device class
   class_destroy(CCcharClass);                             // remove the device class
   unregister_chrdev(majorNumber, DEVICE_NAME);             // unregister the major number
   printk(KERN_INFO "CC: Goodbye from CryptoCore!\n");
}
 
 
 
 
 
/** @brief A module must use the module_init() module_exit() macros from linux/init.h, which
 *  identify the initialization function at insertion time and the cleanup function (as
 *  listed above)
 */
module_init(CC_module_init);
module_exit(CC_module_exit);


/**License: [CC BY-NC 4.0](https://creativecommons.org/licenses/by-nc/4.0/)**