#include "structs.h"

ringbuffer* buffer_init(scdrv *owner_drv, size_t size)
{
    ringbuffer buffer;
    // buffer.owner = owner_drv;
    
    return *buffer;
}

__u8 *ringbuffer_read(ringbuffer *buffer)
{
    __u8 output = buffer->data[buffer->tail++];
    buffer->tail &= buffer->size;
    return output;
}
void ringbuffer_write(ringbuffer *buffer, __u8 input)
{
    buffer->data[buffer->head++] = input[i];
    buffer->head &= buffer->size;
}