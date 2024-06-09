#define MODULE
#define __KERNEL__

#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>

#include "driver.h"

unsigned int major;
const unsigned int minor = 1;
const char *device_name = "scdrv";

struct cdev scdrv_cdev;

dev_t dev;
size_t buffer_size = 1024;

// for linux API
struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = drv_read,
    .write = drv_write,
    .unlocked_ioctl = drv_ioctl,
}

module_init(scdrv_init);
module_exit(scdrv_exit);

// set parameter that can be edited
module_param(buffer_size, __u16, S_IRUGO);

MODULE_AUTHOR("Bogdan Bescherepov");
MODULE_DESCRIPTION("Simple char driver (scdrv)");
MODULE_LICENSE("GPL v2");
MODULE_PARM_DESC(buffer_size, "Buffer size");