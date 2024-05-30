#include "driver.h"
#define SCDRV_IOCTL_OPEN _IO('k', 1)
#define SCDRV_IOCTL_WRITE _IOW('k', 2, uint8_t *)
#define SCDRV_IOCTL_READ _IOR('k', 3, size_t)

bool read(const char *file, unsigned int size) {

};
bool drv_open(const) {}
bool write(const char *file, unsigned int size);

bool set_buffer_size(scdrv drv)
{
    return true;
}

time_t get_last_read_op_time(scdrv drv)
{
    return drv.last_read_time;
}
time_t get_last_write_op_time(scdrv drv)
{
    return drv.last_write_time;
}
time_t update_last_read_op_time(scdrv drv)
{
    drv.last_read_time = time(NULL);
}
time_t update_last_write_op_time(scdrv drv)
{
    drv.last_write_time = time(NULL);
}

pid_t *get_last_read_pid(scdrv drv)
{
    return drv.last_read_pid;
}
uid_t *get_last_read_uid(scdrv drv)
{
    return drv.last_read_uid;
}
pid_t *get_last_write_pid(scdrv drv)
{
    return drv.last_write_pid;
}
uid_t *get_last_write_uid(scdrv drv)
{
    return drv.last_write_uid;
}

void *update_last_read_pid(scdrv drv)
{
    drv.last_read_pid = getpid();
}
void *update_last_read_uid(scdrv drv)
{
    drv.last_read_uid = 1;
}
void *update_last_write_pid(scdrv drv)
{
    drv.last_write_pid = getpid();
}
void *update_last_write_uid(scdrv drv)
{
    drv.last_write_uid = 1;
}

void *set_blocking(scdrv drv, bool should_be_blocked)
{
    int flags = fcntl(drv.fd, F_GETFL, 0);
    if (should_be_blocked)
        flags &= ~O_NONBLOCK;
    else
        flags |= O_NONBLOCK;
    fcntl(drv.fd, F_SETFL, flags);
}

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

struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = drv_open,
    .write = write,
    .open = drv_open,
    // .release = #TODO
    // .close = #TOTO
    .unlocked_ioctl = drv_ioctl,
};

long drv_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch (cmd)
    {
    case SCDRV_IOCTL_OPEN:
        break;

    case SCDRV_IOCTL_WRITE:
    {
        if (loopbuffer_write(&drv.buffer, (uint8_t *)arg) != 0)
            return -EFAULT;
    }
    break;

    case SCDRV_IOCTL_READ:
    {
        uint8_t *data = loopbuffer_read(&drv.buffer, (size_t *)arg);
        if (copy_to_user((long *)arg, data, sizeof(long)) != 0)
            return -EFAULT;
    }
    break;

    default:
        return -ENOTTY;
    }

    return 0;
}