#include "scdrv_io.h"

struct file_operations fops = {
    .owner =    THIS_MODULE,
    .open =     scdrv_fops_open,
    .release =  scdrv_fops_release,
    .read =     scdrv_fops_read,
    .write =    scdrv_fops_write,
    .unlocked_ioctl = scdrv_ioctl,
};

int     is_blocking = true;
bool    is_processing = false;
struct  timespec64 last_write_time = (struct timespec64){0,0};
struct  timespec64 last_read_time = (struct timespec64){0,0};
pid_t   last_write_pid = 0;
uid_t   last_write_uid = 0;
pid_t   last_read_pid = 0;
uid_t   last_read_uid = 0;


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
    // read operation called during another read/write operation in blocking mode
    if (is_blocking && is_processing){
        return -EAGAIN;
    }
    printk(KERN_INFO "SCDRV: Applying read file operation. Output data first byte: %s\n", scdrv_buf.data[scdrv_buf.tail]);
    printk("SCDRV: head= %d, tail= %d\n", scdrv_buf.tail, scdrv_buf.head);
    is_processing = true;

    int bytes_read = 0;

    // end of read
    while (scdrv_buf.tail != scdrv_buf.head){
        put_user(ringbuffer_read(), buf++);
        bytes_read++;
    }
    *off += bytes_read;

    ktime_get_ts64(&last_read_time);
    const struct cred *cred = current_cred();
    last_read_pid = current->pid;
    last_read_uid = cred->uid.val;
    is_processing = false;
    printk(KERN_INFO "SCDRV: Last read time = %d, pid = %d, uid = %d\n", last_read_time, last_read_pid, last_read_uid);
    return bytes_read;
}


ssize_t scdrv_fops_write(struct file *fd, const char *buf, size_t len, loff_t *off)
{
    // write operation called during another read/write operation in blocking mode
    if (is_blocking && is_processing){
        return -EAGAIN;
    }
    // process data
    is_processing = true;
    printk(KERN_INFO "SCDRV: Applying write file operation. Input data: %d\n", buf[0]);
    char input;
    int i = 0;
    while (i < len-1){
        // copy_from_user(&input, , 1);
        ringbuffer_write(buf[i]);
        i++;
    }
    ktime_get_ts64(&last_write_time);

    const struct cred *cred = current_cred();
    last_write_pid = current->pid;
    last_write_uid = cred->uid.val;
    printk(KERN_INFO "SCDRV: Last write time = %d, pid = %d, uid = %d\n", last_write_time, last_write_pid, last_write_uid);
    is_processing = false;
    return len;
}


long scdrv_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    if (_IOC_TYPE(cmd) != SCDRV_IOCTL_NUM) return -EINVAL;
    printk(KERN_INFO "SCDRV: ioctl called. cmd = %d\n", cmd);
    switch (cmd)
    {
        // set driver blocking mode
        case SCDRV_IOCTL_SET_IO_BLOCKING:
        {
            copy_from_user(&is_blocking, (int *) arg, sizeof(is_blocking));
            printk(KERN_INFO "SCDRV: Set blocking to %s\n", is_blocking? "true" : "false");
            return SUCCESS;
        }
        break;


        // send time of last read operation 
        case SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_TIME:
        {
            // printk(KERN_INFO "SCDRV: Last read time = %s\n", last_read_time.tv_sec);
            long int output;
            copy_to_user((long int *)arg, &last_read_time, sizeof(last_read_time.tv_sec));
            return SUCCESS;
        }
        break;

        // send owner's process id of last read operation 
        case SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_PID:
        {
            copy_to_user((int *)arg, &last_read_pid, sizeof(last_read_pid));
            return SUCCESS;
        }
        break;

        // send time of last write operation
        case SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_TIME:
        {
            copy_to_user((long int *)arg, &last_write_time, sizeof(last_write_time.tv_sec));
            return SUCCESS;
        }
        break;

        // send owner's process id of last write operation 
        case SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_PID:
        {
            copy_to_user((void *)arg, &last_write_pid, sizeof(last_write_pid));
            return SUCCESS;
        }
        break;

        // send owner's user id of last read operation 
        case SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_UID:
        {
            copy_to_user((void *)arg, &last_read_uid, sizeof(last_read_uid));
            return SUCCESS;
        }
        break;

        // send owner's user id of last write operation 
        case SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_UID:
        {
            copy_to_user((void *)arg, &last_write_uid, sizeof(last_write_uid));
            return SUCCESS;
        }
        break;

        default:
        {
            return -EINVAL;
        }
    }
    return 0;
}
