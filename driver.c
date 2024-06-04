
#include "buffer.c"

#include <linux/fcntl.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cred.h>
#include <linux/time.h>


#define SCDRV_IOCTL_OPEN _IO('k', 1)
#define SCDRV_IOCTL_WRITE _IOW('k', 2, __u8 *)
#define SCDRV_IOCTL_READ _IOR('k', 3, __u16)

typedef struct
{
    ringbuffer buffer;
    int fd;

    long last_write_time;
    long last_read_time;

    pid_t last_write_pid;
    uid_t last_write_uid;

    pid_t last_read_pid;
    uid_t last_read_uid;

} scdrv;

scdrv drv;

bool set_buffer_size(scdrv drv)
{
    return true;
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

// void set_blocking(scdrv drv, bool should_be_blocked)
// {
//     int flags = fcntl(drv.fd, F_GETFL, 0);
//     if (should_be_blocked)
//         flags &= ~O_NONBLOCK;
//     else
//         flags |= O_NONBLOCK;
//     fcntl(drv.fd, F_SETFL, flags);
// }

scdrv scdrv_init(size_t buffer_size)
{
    scdrv drv;
    drv.buffer = buffer_init(buffer_size);
    drv.last_write_time = 0;
    drv.last_read_time = 0;
    drv.last_write_pid = 0;
    drv.last_write_uid = 0;
    drv.last_read_pid = 0;
    drv.last_read_uid = 0;
    return drv;
}


ssize_t drv_read(struct file * fd, char *buff, size_t len,  loff_t *off) {
    return true;
}
ssize_t drv_write(struct file * fd, const char *buff, size_t len,  loff_t *off){
    return true;
}
ssize_t drv_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch (cmd)
    {
        case SCDRV_IOCTL_OPEN:
            break;

        case SCDRV_IOCTL_WRITE:
        {
            if (loopbuffer_write(&drv.buffer, (__u8 *)arg) != 0)
                return -EFAULT;
        }
        break;

        case SCDRV_IOCTL_READ:
        {
            __u8 data = loopbuffer_read(&drv.buffer, (size_t *)arg);
            if (copy_to_user(&arg, data, sizeof(long)) != 0)
                return -EFAULT;
        }
        break;

        default:
            return -ENOTTY;
        }

    return 0;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = drv_read,
    .write = drv_write,
    .unlocked_ioctl = drv_ioctl,
};

