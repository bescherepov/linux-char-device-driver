#include "structs.h"

__u8 ringbuffer_read()
{
    __u8 output = scdrv_buf.data[scdrv_buf.tail++];
    scdrv_buf.tail &= scdrv_buf.size;
    return output;
}
void ringbuffer_write(__u8 input)
{
    scdrv_buf.data[scdrv_buf.head++] = input;
    scdrv_buf.head &= scdrv_buf.size;
}