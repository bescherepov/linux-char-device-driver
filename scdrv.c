#include "scdrv.h"
#include <linux/slab.h>

int init_module(void)
{
    printk(KERN_INFO "SCDRV: Initializing scdrv module. Buffer size = %d\n", bufsize);

    int rv = alloc_chrdev_region(&dev, 1, minor, device_name);
    if (rv < 0)
    {
        printk("SCDRV: %s failed with %d\n", "Allocating char device region", rv);
        return rv;
    }
    major = MAJOR(dev);

    
    // drv setup
    drv.last_write_time = 0;
    drv.last_read_time = 0;
    drv.last_write_pid = 0;
    drv.last_write_uid = 0;
    drv.last_read_pid = 0;
    drv.last_read_uid = 0;

    // buffer setup
    buffer.size = bufsize;
    buffer.head = 0;
    buffer.tail = 0;
    buffer.data = kmalloc(sizeof(__u8) * bufsize, GFP_KERNEL);
    
    if (rv < 0)
    {
        printk("SCDRV: %s failed with %d\n", "Registering driver", rv);
        return rv;
    }

    cdev_init(&scdrv_cdev, &fops);
    scdrv_cdev.owner = THIS_MODULE;

    rv = cdev_add(&scdrv_cdev, dev, 1);
    if (rv < 0)
    {
        printk(KERN_ERR "SCDRV: Failed to add character device\n");
        unregister_chrdev_region(dev, 1);
        return rv;
    }

    printk(KERN_INFO "SCDRV: Successful module initialization\n");
    return 0;
}

void cleanup_module(void)
{
    printk(KERN_INFO "SCDRV: Closing module\n");
    cdev_del(&scdrv_cdev);
    unregister_chrdev_region(MKDEV(MAJOR(dev), MINOR(dev)), 1);
}