#ifndef SCDRV_IO
#define SCDRV_IO
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <asm/current.h> // fops_read, fops_write

#include "structs.h"
#define SUCCESS 0

#define     SCDRV_IOCTL_SET_IO_BLOCKING _IOW('k', 1, bool)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_TIME _IOR('k', 2, int)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_PID _IOR('k', 3, int)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_TIME _IOR('k', 4, int)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_PID _IOR('k', 5, int)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_UID _IOR('k', 6, int)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_UID _IOR('k', 7, int)

static struct file_operations fops;

// driver file operations
static int 	scdrv_fops_open(struct inode *inode, struct file *file);
static int 	scdrv_fops_release(struct inode *inode, struct file *file);
static ssize_t scdrv_fops_read(struct file *fd, char *buff, size_t len, loff_t *off);
static ssize_t scdrv_fops_write(struct file *fd, const char *buff, size_t len, loff_t *off);

// ioctl
static long    scdrv_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static void    scdrv_set_blocking(scdrv* drv, bool should_be_blocked);

#endif