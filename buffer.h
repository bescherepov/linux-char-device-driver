#ifndef SCDRV_BUFFER
#define SCDRV_BUFFER
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>

typedef struct
{
    __u8 *data;             // bytes of data stored in buffer
    size_t size;            // size of data array
    size_t head;            // index of reading pointer
    size_t tail;            // index of writing pointer
    size_t bytes_available; // subtraction of tail and head values
} ringbuffer;

/// @brief  Calculate and update `buffer`'s "avaliable bytes" variable.
/// @param buffer Buffer to update its "avaliable bytes" variable.
void (*calc_buffer_bytes_available)(ringbuffer *buffer);

/// @brief  Read data from `buffer` ringbuffer.
/// Accessing data starts at `buffer.head`
/// position of `buffer.data` (read how ring buffer works).
/// @returns Pointer to array
/// with `len` length storing part of `buffer`'s data
/// if `len` <= `buffer.size`,
/// otherwise `nullptr` is returned
__u8 *(*loopbuffer_read)(ringbuffer *buffer, size_t len);

/// @brief
int (*loopbuffer_write)(ringbuffer *buffer, __u8 *input);

///
bool (*is_read_allowed)(ringbuffer *buffer, size_t len);

/// @brief
ringbuffer (*buffer_init)(size_t size);
#endif
