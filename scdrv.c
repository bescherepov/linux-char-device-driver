#include "scdrv.h"

int __init init_module(void)
{
    printk(KERN_INFO "SCDRV: Initializing scdrv module. Buffer size = %d\n", bufsize);
    
    int rv = alloc_chrdev_region(&dev, 0, 1, device_name);
    if (rv < 0)
    {
        printk("SCDRV: %s failed with %d\n", "Allocating char device region", rv);
        return rv;
    }
    
    major = MAJOR(dev);
    printk(KERN_INFO "SCDRV: Major number = %d\n", major);

    scdrv_cdev_class = class_create(THIS_MODULE, device_name);
    if (IS_ERR(scdrv_cdev_class))
    {
        printk(KERN_ERR "SCDRV: Failed to create class\n");
        unregister_chrdev_region(dev, 1);
        return PTR_ERR(scdrv_cdev_class);
    }

    struct device *dev_create_code;
    dev_create_code = device_create(scdrv_cdev_class, NULL, MKDEV(major, 0), NULL, "scdrv0");
    if (IS_ERR(dev_create_code))
    {
        printk(KERN_ERR "SCDRV: Failed to create device\n");
        class_destroy(scdrv_cdev_class);
        unregister_chrdev_region(dev, 1);
        return PTR_ERR(dev_create_code);
    }

    cdev_init(&scdrv_cdev, &fops);
    scdrv_cdev.owner = THIS_MODULE;
    scdrv_cdev.ops = &fops;

    rv = cdev_add(&scdrv_cdev, dev, 1);
    if (rv < 0)
    {
        printk(KERN_ERR "SCDRV: Failed to add character device\n");
        unregister_chrdev_region(dev, 1);
        return rv;
    }
    // drv setup
    drv.last_write_time = 0;
    drv.last_read_time = 0;
    drv.last_write_pid = 0;
    // drv.last_write_uid = 0;
    drv.last_read_pid = 0;
    // drv.last_read_uid = 0;

    // buffer setup
    buffer.size = bufsize;
    buffer.head = 0;
    buffer.tail = 0;
    buffer.data = kmalloc(sizeof(__u8) * bufsize, GFP_KERNEL);

    printk(KERN_INFO "SCDRV: Successful module initialization\n");
    return 0;
}

void __exit cleanup_module(void)
{
    printk(KERN_INFO "SCDRV: Closing module\n");
    device_destroy(scdrv_cdev_class, MKDEV(major, 0));
    class_unregister(scdrv_cdev_class);
    class_destroy(scdrv_cdev_class);
    cdev_del(&scdrv_cdev);
    unregister_chrdev_region(MKDEV(MAJOR(dev), MINOR(dev)), 1);
}