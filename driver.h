/* scdrv.h */
#include "buffer.c"
/**
 * @brief Структура для символьного драйвера
 *
 * @param buffer кольцевой буфер, хранящий данные
 * @param READ_BLOCKING отвечает за блокировку считывания данных
 * @param WRITE_BLOCKING List of arguments
 * @param last_write_time времени последней операции чтения
 * @param last_read_time времени последней операции записи
 * @param last_write_pid
 * @param last_write_uid
 * @param last_read_pid
 * @param last_read_uid
 *
 *
 *
 * @return Program exit status
 */
typedef struct
{
    ringbuffer buffer;
    int fd;

    time_t last_write_time;
    time_t last_read_time;

    pid_t last_write_pid;
    uid_t last_write_uid;

    pid_t last_read_pid;
    uid_t last_read_uid;

} scdrv;

scdrv drv;

scdrv scdrv_init(size_t buffer_size);

int (*drv_release)(void);

bool (*set_buffer_size)(scdrv buff);

time_t (*get_last_read_op_time)(scdrv drv);
time_t (*get_last_write_op_time)(scdrv drv);
time_t (*update_last_read_op_time)(scdrv drv);
time_t (*update_last_write_op_time)(scdrv drv);

pid_t (*get_last_read_pid)(scdrv drv);
uid_t (*get_last_read_uid)(scdrv drv);
pid_t (*get_last_write_pid)(scdrv drv);
uid_t (*get_last_write_uid)(scdrv drv);

pid_t (*update_last_read_pid)(scdrv drv);
uid_t (*update_last_read_uid)(scdrv drv);
pid_t (*update_last_write_pid)(scdrv drv);
uid_t (*update_last_write_uid)(scdrv drv);

void (*set_blocking)(void);

struct file_operations fops;
long drv_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
