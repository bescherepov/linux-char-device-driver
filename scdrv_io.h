#ifndef SCDRV_IO
#define SCDRV_IO
#include "structs.h"

#include <asm/current.h> // fops_read, fops_write
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/slab.h>


#define 	SUCCESS 0
#define 	SCDRV_IOCTL_NUM 0x11

#define     SCDRV_IOCTL_SET_IO_BLOCKING 				_IOW(SCDRV_IOCTL_NUM, 1, bool)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_TIME 	_IOR(SCDRV_IOCTL_NUM, 2, int)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_PID 	_IOR(SCDRV_IOCTL_NUM, 3, int)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_TIME 	_IOR(SCDRV_IOCTL_NUM, 4, int)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_PID 	_IOR(SCDRV_IOCTL_NUM, 5, int)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_UID 	_IOR(SCDRV_IOCTL_NUM, 6, int)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_UID 	_IOR(SCDRV_IOCTL_NUM, 7, int)

// driver file operations
extern int 	scdrv_fops_open(struct inode *inode, struct file *file);
extern int 	scdrv_fops_release(struct inode *inode, struct file *file);
extern ssize_t scdrv_fops_read(struct file *fd, char *buff, size_t len, loff_t *off);
extern ssize_t scdrv_fops_write(struct file *fd, const char *buff, size_t len, loff_t *off);

// ioctl
extern long    scdrv_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
extern void    scdrv_set_blocking(scdrv* drv, bool should_be_blocked);

extern struct file_operations fops;

#endif