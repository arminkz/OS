#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define  DEVICE_NAME "akpchar"
#define  CLASS_NAME  "AKP"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Armin Kazemi");
MODULE_DESCRIPTION("A simple Linux char driver for OSLab Project");
MODULE_VERSION("0.1");

static char*  mystr = "armin\0";
static char   message[256] = {0}; 

static int    majorNumber;
static struct class*  akpCharClass  = NULL;
static struct device* akpCharDevice = NULL;

// The prototype functions for the character driver -- must come before the struct definition
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

static int __init akpchar_init(void){
   printk(KERN_INFO "AKPChar: Initializing the AKPChar LKM\n");

   // Try to dynamically allocate a major number for the device
   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   if (majorNumber<0){
      printk(KERN_ALERT "AKPChar: failed to register a major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "AKPChar: registered correctly with major number %d\n", majorNumber);

   // Register the device class
   akpCharClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(akpCharClass)){
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(akpCharClass);
   }
   printk(KERN_INFO "AKPChar: device class registered correctly\n");

   // Register the device driver
   akpCharDevice = device_create(akpCharClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(akpCharDevice)){
      class_destroy(akpCharClass);
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(akpCharDevice);
   }
   printk(KERN_INFO "AKPChar: device class created correctly\n"); // Made it! device was initialized
   return 0;
}

static void __exit akpchar_exit(void){
   device_destroy(akpCharClass, MKDEV(majorNumber, 0));
   class_unregister(akpCharClass);
   class_destroy(akpCharClass);
   unregister_chrdev(majorNumber, DEVICE_NAME);
   printk(KERN_INFO "AKPChar: Goodbye from the LKM!\n");
}

static int dev_open(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "AKPChar: Device has been opened !\n");
   return 0;
}

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

static int dev_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "AKPChar: Device successfully closed\n");
   return 0;
}

module_init(akpchar_init);
module_exit(akpchar_exit);
