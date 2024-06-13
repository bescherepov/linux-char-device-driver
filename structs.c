#include "structs.h"

__u8 ringbuffer_read(ringbuffer *buffer)
{
    __u8 output = buffer->data[buffer->tail++];
    buffer->tail &= buffer->size;
    return output;
}
void ringbuffer_write(ringbuffer *buffer, __u8 input)
{
    buffer->data[buffer->head++] = input;
    buffer->head &= buffer->size;
}