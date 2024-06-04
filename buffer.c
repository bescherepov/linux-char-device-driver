#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>

typedef struct
{
    __u8 *data;
    size_t size;
    size_t head;
    size_t tail;
    size_t bytes_available;
} ringbuffer;

void calc_buffer_bytes_available(ringbuffer *buffer)
{
    if (buffer->head < buffer->tail)
        buffer->bytes_available = buffer->size - buffer->tail + buffer->head;
    else
        buffer->bytes_available = buffer->head - buffer->tail;
}

__u8 *loopbuffer_read(ringbuffer *buffer, size_t len)
{
    __u8 output[len];

    int i = 0;
    for (i; i < len; i++)
    {
        output[i] = buffer->data[i + buffer->tail++];
        buffer->tail &= buffer->size;
    }

    calc_buffer_bytes_available(buffer);
    return output;
}
int loopbuffer_write(ringbuffer *buffer, __u8 *input)
{
    size_t len = sizeof(input) / sizeof(__u8);
    int i = 0;
    for (i; i < len; i++)
    {
        buffer->data[buffer->head] = input[i];
        buffer->head &= buffer->size;
    }
    calc_buffer_bytes_available(buffer);
    return 0;
}



bool is_read_allowed(ringbuffer *buffer, size_t len)
{
    return len <= buffer->bytes_available;
}

ringbuffer buffer_init(size_t size)
{
    ringbuffer buffer;
    buffer.size = size;
    buffer.head = 0;
    buffer.tail = 0;
    buffer.bytes_available = 0;
    buffer.data = kmalloc(sizeof(size_t) * 4 + sizeof(__u8) * size, GFP_KERNEL);
    return buffer;
}
