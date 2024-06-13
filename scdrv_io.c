#include "scdrv_io.h"

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = scdrv_fops_open,
    .release = scdrv_fops_release,
    .read = scdrv_fops_read,
    .write = scdrv_fops_write,
    .unlocked_ioctl = scdrv_ioctl,
};
void scdrv_set_blocking(scdrv *drv, bool should_be_blocked)
{
    // int flags = fcntl(drv.fd, F_GETFL, 0);
    // if (should_be_blocked)
    //     flags &= ~O_NONBLOCK;
    // else
    //     flags |= O_NONBLOCK;
    // fcntl(drv.fd, F_SETFL, flags);
}
int scdrv_fops_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "SCDRV: scdrv opened\n");
    return SUCCESS;
}

int scdrv_fops_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "SCDRV: scdrv released\n");
    return SUCCESS;
}

ssize_t scdrv_fops_read(struct file *fd, char *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "SCDRV: Applying read file operation. Input data: %s\n", buf);
    int i = 0;
    while (i < len-1){
        copy_to_user(&buf[i], ringbuffer_read(), 1);
        ringbuffer_read();
        i++;
    }
    drv.last_read_time = ktime_get_real();
    drv.last_read_pid = current->pid;
    drv.last_read_uid = current_uid();
    return SUCCESS;
}

ssize_t scdrv_fops_write(struct file *fd, const char *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "SCDRV: Applying write file operation. Input data: %d\n", (int)buf[0]);
    __u8* input = kmalloc(sizeof(__u8), GFP_KERNEL);
    int i = 0;
    while (i < len-1){
        copy_from_user(input, &buf[i], 1);
        ringbuffer_write(input);
        i++;
    }
    drv.last_write_time = ktime_get_real();
    drv.last_write_pid = current->pid;
    drv.last_write_uid = current_uid();
    return len;
}

long scdrv_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    printk(KERN_INFO "SCDRV: ioctl called\n");
    switch (cmd)
    {
        case SCDRV_IOCTL_SET_IO_BLOCKING:
        {
            scdrv_set_blocking(&drv, (bool)arg);
            return SUCCESS;
        }
        break;

        case SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_TIME:
        {
            copy_to_user(&arg, &drv.last_read_time, sizeof(drv.last_read_time));
            return SUCCESS;
        }
        break;

        case SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_PID:
        {
            copy_to_user(&arg, &drv.last_read_pid, sizeof(drv.last_read_pid));
            return SUCCESS;
        }
        break;

        case SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_TIME:
        {
            copy_to_user(&arg, &drv.last_write_time, sizeof(drv.last_write_time));
            return SUCCESS;
        }
        break;

        case SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_PID:
        {
            copy_to_user(&arg, &drv.last_write_pid, sizeof(drv.last_write_pid));
            return SUCCESS;
        }
        break;

        case SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_UID:
        {
            copy_to_user(&arg, &drv.last_read_uid, sizeof(drv.last_read_uid));
            return SUCCESS;
        }
        break;

        case SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_UID:
        {
            copy_to_user(&arg, &drv.last_write_uid, sizeof(drv.last_write_uid));
            return SUCCESS;
        }
        break;

        default:
        {
            return -ENOTTY;
        
        }
    }
    return 0;
}

