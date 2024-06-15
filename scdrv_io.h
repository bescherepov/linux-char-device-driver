#ifndef SCDRV_IO
#define SCDRV_IO
#include "buffer.h"

#include <asm/current.h> // fops_read, fops_write
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/timekeeping.h>
#include <linux/sched.h>

#define 	SUCCESS 0
#define 	SCDRV_IOCTL_NUM 0x11

#define     SCDRV_IOCTL_SET_IO_BLOCKING 				_IOW(SCDRV_IOCTL_NUM, 1, bool)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_TIME 	_IOR(SCDRV_IOCTL_NUM, 2, long int)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_PID 	_IOR(SCDRV_IOCTL_NUM, 3, int)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_TIME 	_IOR(SCDRV_IOCTL_NUM, 4, long int)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_PID 	_IOR(SCDRV_IOCTL_NUM, 5, int)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_UID 	_IOR(SCDRV_IOCTL_NUM, 6, int)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_UID 	_IOR(SCDRV_IOCTL_NUM, 7, int)

// driver file operations
extern int 		scdrv_fops_open(struct inode *inode, struct file *file);
extern int 		scdrv_fops_release(struct inode *inode, struct file *file);
extern ssize_t 	scdrv_fops_read(struct file *fd, char *buff, size_t len, loff_t *off);
extern ssize_t 	scdrv_fops_write(struct file *fd, const char *buff, size_t len, loff_t *off);

extern struct 	timespec64 last_write_time; // time of last write operation
extern struct 	timespec64 last_read_time; // time of last read operation

extern pid_t   	last_write_pid;
extern uid_t   	last_write_uid;

extern pid_t   	last_read_pid;
extern uid_t   	last_read_uid;

extern int 		is_blocking; // determines whether read/write operations is blocking or not;
extern bool    	is_processing; // indicates whether read/write operation is processing at moment;

// ioctl
extern long    	scdrv_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

extern struct file_operations fops;

#endif