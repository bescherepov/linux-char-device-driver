#ifndef SCDRV_DRIVER
#define SCDRV_DRIVER
#include "buffer.c"

#include <linux/cred.h>
#include <linux/fcntl.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/types.h>
#include <linux/uaccess.h>

// Macros for ioctl
#define SCDRV_IOCTL_OPEN _IO('k', 1)
#define SCDRV_IOCTL_WRITE _IOW('k', 2, __u8 *)
#define SCDRV_IOCTL_READ _IOR('k', 3, __u16)

/// @brief Driver structure.
/// @param buffer object that stores data
/// with ring buffer structure.
typedef struct
{
    ringbuffer buffer;

    long last_write_time;
    long last_read_time;

    pid_t last_write_pid;
    uid_t last_write_uid;

    pid_t last_read_pid;
    uid_t last_read_uid;

} scdrv;

// for module_init and module_exit of <linux/module.h>
int __init (*scdrv_init)(void);
void __exit (*scdrv_exit)(void);

// driver file operations
ssize_t (*scdrv_open)(struct inode *inode, struct file *file);
ssize_t (*scdrv_release)(struct inode *inode, struct file *file);
ssize_t (*scdrv_read)(struct file *fd, char *buff, size_t len, loff_t *off);
ssize_t (*scdrv_write)(struct file *fd, const char *buff, size_t len, loff_t *off);
ssize_t (*scdrv_ioctl)(struct file *file, unsigned int cmd, unsigned long arg);

scdrv drv = {
    .buffer = buffer_init(buffer_size);
.last_write_time = 0;
.last_read_time = 0;
.last_write_pid = 0;
.last_write_uid = 0;
.last_read_pid = 0;
.last_read_uid = 0;
}
;


// void set_blocking(scdrv drv, bool should_be_blocked)
// {
//     int flags = fcntl(.fd, F_GETFL, 0);
//     if (should_be_blocked)
//         flags &= ~O_NONBLOCK;
//     else
//         flags |= O_NONBLOCK;
//     fcntl(.fd, F_SETFL, flags);
// }
#endif
