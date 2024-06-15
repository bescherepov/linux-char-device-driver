#ifndef SCDRV_BUFFER
#define SCDRV_BUFFER
#include <linux/cred.h>
#include <linux/types.h>
#include <linux/slab.h>

// ringbuffer
typedef struct
{
    char    *data;  // bytes of data stored in buffer
    int     size; // size of data array
    int     head; // index of reading pointer
    int     tail; // index of writing pointer
} ringbuffer;

extern int bufsize;
extern ringbuffer scdrv_buf;

extern char ringbuffer_read(void); // read byte of data
extern void ringbuffer_write(char input); // write byte of data

#endif