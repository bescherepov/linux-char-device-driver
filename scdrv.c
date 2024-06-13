#include "scdrv.h"

int __init init_module(void)
{
    printk(KERN_INFO "SCDRV: Initializing scdrv module. Buffer size = %d\n", bufsize);
    
    // allocate chardev region for scdrv
    int rv = alloc_chrdev_region(&dev, 0, 1, SCDRV_DEVICE_NAME);
    if (rv < 0)
    {
        printk(KERN_ERR "SCDRV: %s failed with %d\n", "Allocating char device region", rv);
        return rv;
    }
    
    // update local major and minor numbers after dynamic generation
    major = MAJOR(dev); minor = MINOR(dev);
    printk(KERN_INFO "SCDRV: Major number = %d, minor = %d\n", major, minor);

    // init char device with file operations
    cdev_init(&scdrv_cdev, &fops);

    // add new char device
    rv = cdev_add(&scdrv_cdev, dev, 1);
    if (rv < 0)
    {
        printk(KERN_ERR "%s: Failed to add the device to the system\n", SCDRV_DEVICE_NAME);
        return rv;
    }

    scdrv_cdev_class = class_create(THIS_MODULE, SCDRV_CLASS_NAME);
    if (IS_ERR(scdrv_cdev_class))
    {
        printk(KERN_ERR "SCDRV: Failed to create class\n");
        return PTR_ERR(scdrv_cdev_class);
    }

    // create new device file in system
    struct device *scdrv_device;
    scdrv_device = device_create(scdrv_cdev_class, NULL, MKDEV(major, 0), NULL, "scdrv0");
    if (IS_ERR(scdrv_device))
    {
        printk(KERN_ERR "SCDRV: Failed to create device\n");
        class_destroy(scdrv_cdev_class);
        return PTR_ERR(scdrv_device);
    }

    // drv setup
    drv.last_write_time = 0;
    drv.last_read_time = 0;
    drv.last_write_pid = 0;
    // drv.last_write_uid = 0;
    drv.last_read_pid = 0;
    // drv.last_read_uid = 0;

    // buffer setup
    scdrv_buf.size = bufsize;
    scdrv_buf.head = 0;
    scdrv_buf.tail = 0;
    scdrv_buf.data = kmalloc(sizeof(__u8) * bufsize, GFP_KERNEL);

    // int i;
    // for (i = 0; i < bufsize; ++i)
    // {
    //     printk("%d", scdrv_buf.data[i]);
    //     printk("\n");
    // }
    printk(KERN_INFO "SCDRV: Successful module initialization\n");
    return 0;
}


void __exit cleanup_module(void)
{
    device_destroy(scdrv_cdev_class, dev);
    class_destroy(scdrv_cdev_class);
    cdev_del(&scdrv_cdev);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "SCDRV: Closing module\n");
}