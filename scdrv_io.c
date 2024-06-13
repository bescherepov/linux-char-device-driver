#include "scdrv_io.h"
void scdrv_set_blocking(scdrv *drv, bool should_be_blocked)
{
    int flags = fcntl(drv.fd, F_GETFL, 0);
    if (should_be_blocked)
        flags &= ~O_NONBLOCK;
    else
        flags |= O_NONBLOCK;
    fcntl(drv.fd, F_SETFL, flags);
}
ssize_t scdrv_fops_open(struct inode *inode, struct file *file)
{
    MOD_INC_USE_COUNT;
    return SUCCESS;
}

ssize_t scdrv_fops_release(struct inode *inode, struct file *file)
{
    MOD_DEC_USE_COUNT;
    return SUCCESS;
}

ssize_t scdrv_fops_read(struct file *fd, char *buf, size_t len, loff_t *off)
{
    if (sizeof(buf) / sizeof(char) < len)
        return -1;
    drv.last_read_time = ktime_get_real();
    drv.last_read_pid = current->pid;
    drv.last_read_uid = current_uid();
    return true;
}

ssize_t scdrv_fops_write(struct file *fd, const char *buf, size_t len, loff_t *off)
{
    drv.last_write_time = ktime_get_real();
    return true;
}

long scdrv_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch (cmd)
    {
        case SCDRV_IOCTL_SET_IO_BLOCKING:
        {
            set_blocking(drv, (bool)arg);
            return SUCCESS;
        }
        break;

        case SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_TIME:
        {
            copy_to_user(*arg, buffer.last_read_time, sizeof(buffer.last_read_time));
            return SUCCESS;
        }
        break;

        case SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_PID:
        {
            copy_to_user(*arg, buffer.last_read_pid, sizeof(buffer.last_read_pid));
            return SUCCESS;
        }
        break;

        case SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_TIME:
        {
            copy_to_user(*arg, buffer.last_write_time, sizeof(buffer.last_write_time));
            return SUCCESS;
        }
        break;

        case SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_PID:
        {
            copy_to_user(*arg, buffer.last_write_pid, sizeof(buffer.last_write_pid));
            return SUCCESS;
        }
        break;

        case SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_UID:
        {
            copy_to_user(*arg, buffer.last_read_uid, sizeof(buffer.last_read_uid));
            return SUCCESS;
        }
        break;

        case SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_UID:
        {
            copy_to_user(*arg, buffer.last_write_uid, sizeof(buffer.last_write_uid));
            return SUCCESS;
        }
        break;

        default:
            return -ENOTTY;
        }
    }
    return 0;
}
SCDRV_IOCTL_SET_IO_BLOCKING
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_TIME _IOR('k', 2)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_PID _IOR('k', 3)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_TIME _IOR('k', 4)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_PID _IOR('k', 5)
// for linux API
struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = scdrv_fops_open,
    .release = scdrv_fops_release,
    .read = scdrv_fops_read,
    .write = scdrv_fops_write,
    .unlocked_ioctl = scdrv_ioctl,
};