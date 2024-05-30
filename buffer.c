#include "libs.h"

typedef struct
{
    uint8_t *data;
    size_t size;
    size_t head;
    size_t tail;
    size_t bytes_available;
} ringbuffer;

uint8_t *loopbuffer_read(ringbuffer *buffer, size_t len)
{
    uint8_t output[len];
    for (int i = 0; i < len; i++)
    {
        output[i] = buffer->data[i + buffer->tail++];
        buffer->tail &= buffer->size;
    }
    calc_buffer_bytes_available(buffer);
    return output;
}
int loopbuffer_write(ringbuffer *buffer, uint8_t *input)
{
    size_t len = sizeof(input) / sizeof(uint8_t);
    for (int i = 0; i < len; i++)
    {
        buffer->data[buffer->head] = input[i];
        buffer->head &= buffer->size;
    }
    calc_buffer_bytes_available(buffer);
    return 0;
}

void calc_buffer_bytes_available(ringbuffer *buffer)
{
    if (buffer->head < buffer->tail)
        buffer->bytes_available = buffer->size - buffer->tail + buffer->head;
    else
        buffer->bytes_available = buffer->head - buffer->tail;
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
    buffer.data = kmalloc(sizeof(size_t) * 4 + sizeof(uint8_t) * size, GFP_KERNEL);
    return buffer;
}
