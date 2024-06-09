
#include "driver.h"
#include "buffer.h"

#define SCDRV_IOCTL_OPEN _IO('k', 1)
#define SCDRV_IOCTL_WRITE _IOW('k', 2, __u8 *)
#define SCDRV_IOCTL_READ _IOR('k', 3, __u16)

bool set_buffer_size(scdrv drv)
{
    return true;
}

// void set_blocking(scdrv drv, bool should_be_blocked)
// {
//     int flags = fcntl(drv.fd, F_GETFL, 0);
//     if (should_be_blocked)
//         flags &= ~O_NONBLOCK;
//     else
//         flags |= O_NONBLOCK;
//     fcntl(drv.fd, F_SETFL, flags);
// }
ssize_t scdrv_open(struct inode *inode, struct file *file)
{
    MOD_INC_USE_COUNT
    return SUCCESS;
}

ssize_t scdrv_release(struct inode *inode, struct file *file)
{
    MOD_DEC_USE_COUNT;
    return SUCCESS;
}

ssize_t scdrv_read(struct file *fd, char *buf, size_t len, loff_t *off)
{

    return true;
}

ssize_t scdrv_write(struct file *fd, const char *buf, size_t len, loff_t *off)
{
    return true;
}

ssize_t scdrv_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch (cmd)
    {
    case SCDRV_IOCTL_OPEN:
        break;

    case SCDRV_IOCTL_WRITE:
    {
        return
    }
    break;

    case SCDRV_IOCTL_READ:
    {
        __u8 data = loopbuffer_read(&drv.buffer, arg);
        if (copy_to_user(&arg, data, sizeof(long)) != 0)
            return -EFAULT;
    }
    break;

    default:
        return -ENOTTY;
    }

    return 0;
}

int get_last_read_op_time(scdrv drv)
{
    return drv.last_read_time;
}
int get_last_write_op_time(scdrv drv)
{
    return drv.last_write_time;
}
void update_last_read_op_time(scdrv drv)
{
    drv.last_read_time = 0;
}
void update_last_write_op_time(scdrv drv)
{
    drv.last_write_time = 0;
}

pid_t get_last_read_pid(scdrv drv)
{
    return drv.last_read_pid;
}
uid_t get_last_read_uid(scdrv drv)
{
    return drv.last_read_uid;
}
pid_t get_last_write_pid(scdrv drv)
{
    return drv.last_write_pid;
}
uid_t get_last_write_uid(scdrv drv)
{
    return drv.last_write_uid;
}

void update_last_read_pid(scdrv drv, struct pid *pid)
{
    drv.last_read_pid = get_pid(pid);
}
void update_last_read_uid(scdrv drv)
{
    drv.last_read_uid = 1;
}
void update_last_write_pid(scdrv drv, struct pid *pid)
{
    drv.last_write_pid = get_pid(pid);
}
void update_last_write_uid(scdrv drv)
{
    drv.last_write_uid = 1;
}