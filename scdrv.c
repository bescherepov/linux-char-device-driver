#include "scdrv.h"

int __init init_module(void)
{
    printk(KERN_INFO "SCDRV: Initializing scdrv module. Buffer size = %d\n", bufsize);
    
    // allocate chardev region for scdrv
    int rv = alloc_chrdev_region(&dev, 0, 1, SCDRV_NAME);
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
        printk(KERN_ERR "SCDRV: Failed to add the device to the system\n");
        return rv;
    }

    // create class
    scdrv_cdev_class = class_create(THIS_MODULE, SCDRV_CLASS_NAME);
    if (IS_ERR(scdrv_cdev_class))
    {
        printk(KERN_ERR "SCDRV: Failed to create class\n");
        return PTR_ERR(scdrv_cdev_class);
    }

    // create new device file in system
    struct device *scdrv_device;
    scdrv_device = device_create(scdrv_cdev_class, NULL, MKDEV(major, 0), NULL, SCDRV_DEVICE_NAME);
    if (IS_ERR(scdrv_device))
    {
        printk(KERN_ERR "SCDRV: Failed to create device\n");
        class_destroy(scdrv_cdev_class);
        return PTR_ERR(scdrv_device);
    }

    //  setup
    scdrv_buf.size = bufsize;
    printk("bufsize = %d\n", scdrv_buf.size);
    scdrv_buf.head = 0;
    scdrv_buf.tail = 0;
    scdrv_buf.data = (char *)kmalloc(sizeof(char) * bufsize, GFP_KERNEL);
    printk("data %p \n", (void *) scdrv_buf.data);
    

    printk(KERN_INFO "SCDRV: Successful module initialization\n");
    return 0;
}


void __exit cleanup_module(void)
{
    printk("data close %p \n", (void *) scdrv_buf.data);
    device_destroy(scdrv_cdev_class, dev);
    class_destroy(scdrv_cdev_class);
    cdev_del(&scdrv_cdev);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "SCDRV: Closing module\n");
}