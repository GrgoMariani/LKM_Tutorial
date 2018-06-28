/* Most of the code taken from example by Derek Molloy  @ http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/
 * mixed w/ https://www.apriorit.com/dev-blog/195-simple-driver-for-linux-os */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define  DEVICE_NAME "OURDEVICE"          ///< The device will appear at /dev/LOOKATTHIS using this value
#define  CLASS_NAME  "DEVICECLASS"        ///< The device class -- this is a character device driver


MODULE_LICENSE("GPL");
MODULE_AUTHOR("GrgoMariani");
MODULE_DESCRIPTION("This module implements basic char device operations");
MODULE_VERSION("0.02");



static int    majorNumber;
static char   message[256] = {0};
static short  size_of_message;
static int    numberOpens = 0;
static struct class*  module_twoClass  = NULL; ///< The device-driver class struct pointer
static struct device* module_twoDevice = NULL; ///< The device-driver device struct pointer

// PROTOTYPES
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops ={   .open = dev_open,   .read = dev_read,   .write = dev_write,   .release = dev_release,  };
/* *************************************************************************************************************************** */



static int __init module_two_init(void){
   printk(KERN_INFO "MODULE_TWO: Initializing the module_two LKM\n");
   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   if (majorNumber<0){
      printk(KERN_ALERT "MODULE_TWO failed to register a major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "MODULE_TWO: registered correctly with major number %d\n", majorNumber);

   // Register the device class - not C++ class :D, we're working in C now
   module_twoClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(module_twoClass)){
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "MODULE_TWO: Failed to register device class\n");
      return PTR_ERR(module_twoClass);          // Correct way to return an error on a pointer
   }
   printk(KERN_INFO "MODULE_TWO: device class registered correctly\n");

   module_twoDevice = device_create(module_twoClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(module_twoDevice)){  // watch your back for all types of possible errors
      class_destroy(module_twoClass);
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "MODULE_TWO: Failed to create the device\n");
      return PTR_ERR(module_twoDevice);
   }
   printk(KERN_INFO "MODULE_TWO: device class created correctly\n");
   return 0;
}


static void __exit module_two_exit(void){
   device_destroy(module_twoClass, MKDEV(majorNumber, 0));
   class_unregister(module_twoClass);
   class_destroy(module_twoClass);
   unregister_chrdev(majorNumber, DEVICE_NAME);
   printk(KERN_INFO "MODULE_TWO: Goodbye from the LKM!\n");
}




static int dev_open(struct inode *inodep, struct file *filep){
   numberOpens++;
   /* We'll usually put mutex_lock in our dev_open code, and mutex_unlock in dev_release */
   printk(KERN_INFO "MODULE_TWO: Device has been opened %d time(s)\n", numberOpens);
   return 0;
}


static int dev_release(struct inode *inodep, struct file *filep){
    /* A good enough place for mutex_unlock */
   printk(KERN_INFO "MODULE_TWO: Device successfully closed\n");
   return 0;
}




static ssize_t dev_read(struct file *filep, char *buffer, size_t count, loff_t *offset){
   int error_count = 0;

   error_count = copy_to_user(buffer, message, size_of_message);

   if (error_count==0){
      printk(KERN_INFO "MODULE_TWO: Sent %d characters to the user. Offset=%i , len=%u\n", size_of_message, (int)*offset, (unsigned int)count);
      short llen = size_of_message;
      size_of_message = 0;
      return llen;  // clear the position to the start and return the length of our message
   }
   else {
      printk(KERN_INFO "MODULE_TWO: Failed to send %d characters to the user\n", error_count);
      return -EFAULT;              // check these files for more  ---- /usr/include/asm-generic/errno-base.h ---- /usr/include/asm-generic/errno.h
   }
}


static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
   sprintf(message, "%s(%zu letters)\n", buffer, len);
   size_of_message = strlen(message);                    // store the length of the stored message
   printk(KERN_INFO "MODULE_TWO: Received %zu characters from the user\n", len);
   return len;
}




module_init(module_two_init);
module_exit(module_two_exit);