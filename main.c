#include "scdrv_module.h"

int scdrvmod_init(void)
{
    printk(KERN_INFO "SCDRV: Initializing scdrv module\n");

    int rv = alloc_chrdev_region(&dev, 1, minor, device_name);
    major = MAJOR(dev);

    drv = scdrv_init(buffer_size);
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
    return 0;
}

void scdrvmod_exit(void)
{
    printk(KERN_INFO "SCDRV: Exiting scdrv module\n");
    cdev_del(&scdrv_cdev);
    unregister_chrdev_region(MKDEV(MAJOR(dev), MINOR(dev)), 1);
}
