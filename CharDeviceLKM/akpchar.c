/**
 * @file   akpchar.c
 * @author Armin Kazemi
 * @date   4 Jan 2018
 * @version 0.1
 * @brief   An introductory character driver to support the second article of my series on
 * Linux loadable kernel module (LKM) development. This module maps to /dev/akpchar and
 * comes with a helper C program that can be run in Linux user space to communicate with
 * this the LKM.
 */

#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <asm/uaccess.h>          // Required for the copy to user function

#define  DEVICE_NAME "akpchar"    ///< The device will appear at /dev/akpchar using this value
#define  CLASS_NAME  "AKP"        ///< The device class -- this is a character device driver

MODULE_LICENSE("GPL");            ///< The license type -- this affects available functionality
MODULE_AUTHOR("Armin Kazemi");    ///< The author -- visible when you use modinfo
MODULE_DESCRIPTION("A simple Linux char driver for OSLab Project");  ///< The description -- see modinfo
MODULE_VERSION("0.1");            ///< A version number to inform users

static char*  mystr = "armin\0";
static char   message[256] = {0}; 

static int    majorNumber;                  ///< Stores the device number -- determined automatically
static struct class*  akpCharClass  = NULL; ///< The device-driver class struct pointer
static struct device* akpCharDevice = NULL; ///< The device-driver device struct pointer

// The prototype functions for the character driver -- must come before the struct definition
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

/** @brief Devices are represented as file structure in the kernel. The file_operations structure from
 *  /linux/fs.h lists the callback functions that you wish to associated with your file operations
 *  using a C99 syntax structure. char devices usually implement open, read, write and release calls
 */
static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

/** @brief The LKM initialization function
 *  The static keyword restricts the visibility of the function to within this C file. The __init
 *  macro means that for a built-in driver (not a LKM) the function is only used at initialization
 *  time and that it can be discarded and its memory freed up after that point.
 *  @return returns 0 if successful
 */
static int __init akpchar_init(void){
   printk(KERN_INFO "AKPChar: Initializing the AKPChar LKM\n");

   // Try to dynamically allocate a major number for the device -- more difficult but worth it
   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   if (majorNumber<0){
      printk(KERN_ALERT "AKPChar: failed to register a major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "AKPChar: registered correctly with major number %d\n", majorNumber);

   // Register the device class
   akpCharClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(akpCharClass)){                // Check for error and clean up if there is
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(akpCharClass);          // Correct way to return an error on a pointer
   }
   printk(KERN_INFO "AKPChar: device class registered correctly\n");

   // Register the device driver
   akpCharDevice = device_create(akpCharClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(akpCharDevice)){               // Clean up if there is an error
      class_destroy(akpCharClass);           // Repeated code but the alternative is goto statements
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(akpCharDevice);
   }
   printk(KERN_INFO "AKPChar: device class created correctly\n"); // Made it! device was initialized
   return 0;
}

/** @brief The LKM cleanup function
 *  Similar to the initialization function, it is static. The __exit macro notifies that if this
 *  code is used for a built-in driver (not a LKM) that this function is not required.
 */
static void __exit akpchar_exit(void){
   device_destroy(akpCharClass, MKDEV(majorNumber, 0));     // remove the device
   class_unregister(akpCharClass);                          // unregister the device class
   class_destroy(akpCharClass);                             // remove the device class
   unregister_chrdev(majorNumber, DEVICE_NAME);             // unregister the major number
   printk(KERN_INFO "AKPChar: Goodbye from the LKM!\n");
}

/** @brief The device open function that is called each time the device is opened
 *  This will only increment the numberOpens counter in this case.
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int dev_open(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "AKPChar: Device has been opened !\n");
   return 0;
}

/** @brief This function is called whenever device is being read from user space i.e. data is
 *  being sent from the device to the user. In this case is uses the copy_to_user() function to
 *  send the buffer string to the user and captures any errors.
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 *  @param buffer The pointer to the buffer to which this function writes the data
 *  @param len The length of the b
 *  @param offset The offset if required
 */
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
    ssize_t cnt = strlen(mystr), ret;

    /* ret contains the amount of chare wasn't successfully written to `buffer` */
    ret = copy_to_user(buffer, mystr, cnt);
    *offset += cnt - ret;

    /* Making sure there are no left bytes of data to send user */
    if (*offset > cnt){
	 printk(KERN_INFO "AKPChar: Sent to the user!\n");
         return 0;
    }else
         return cnt;
}

/** @brief This function is called whenever the device is being written to from user space i.e.
 *  data is sent to the device from the user. The data is copied to the message[] array in this
 *  LKM using the sprintf() function along with the length of the string.
 *  @param filep A pointer to a file object
 *  @param buffer The buffer to that contains the string to write to the device
 *  @param len The length of the array of data that is being passed in the const char buffer
 *  @param offset The offset if required
 */
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
   ssize_t ret = 0;
   ret = copy_from_user (message , buffer, len);

   if(ret>=0){
	printk(KERN_INFO "AKPChar: Recieved from the user!\n");
	if(strcmp(mystr,message)==0){ 
		printk(KERN_INFO "AKPChar: Recieved Correct Word!\n");
		return 0;
	}else return -EINVAL;
	
   }else return ret;
}

/** @brief The device release function that is called whenever the device is closed/released by
 *  the userspace program
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int dev_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "AKPChar: Device successfully closed\n");
   return 0;
}

/** @brief A module must use the module_init() module_exit() macros from linux/init.h, which
 *  identify the initialization function at insertion time and the cleanup function (as
 *  listed above)
 */
module_init(akpchar_init);
module_exit(akpchar_exit);
