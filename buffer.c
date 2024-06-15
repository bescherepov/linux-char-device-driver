#include "buffer.h"

char ringbuffer_read()
{
    printk("data read %d \n", scdrv_buf.data[scdrv_buf.tail]);
    char output = scdrv_buf.data[scdrv_buf.tail++];
    scdrv_buf.tail &= scdrv_buf.size - 1;
    return output;
}
void ringbuffer_write(char input)
{
    printk("data write %d \n", scdrv_buf.data[scdrv_buf.head]);
    scdrv_buf.data[scdrv_buf.head++] = input;
    scdrv_buf.head &= scdrv_buf.size - 1;
}