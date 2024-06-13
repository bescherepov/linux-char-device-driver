#include <linux/fs.h>
#include <asm/current.h> // fops_read, fops_write

#include "structs.h"

#define     SCDRV_IOCTL_SET_IO_BLOCKING _IOW('k', 1, bool)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_TIME _IOR('k', 2)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_PID _IOR('k', 3)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_TIME _IOR('k', 4)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_PID _IOR('k', 5)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_UID _IOR('k', 6)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_UID _IOR('k', 7)

struct file_operations fops;

// driver file operations
int 	scdrv_fops_open(struct inode *inode, struct file *file);
int 	scdrv_fops_release(struct inode *inode, struct file *file);
ssize_t scdrv_fops_read(struct file *fd, char *buff, size_t len, loff_t *off);
ssize_t scdrv_fops_write(struct file *fd, const char *buff, size_t len, loff_t *off);

// ioctl
long        scdrv_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
void        scdrv_set_blocking(scdrv drv, bool should_be_blocked);