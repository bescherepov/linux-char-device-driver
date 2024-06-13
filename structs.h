#ifndef SCDRV_STRUCTURE
#define SCDRV_STRUCTURE
#include <linux/cred.h>
#include <linux/types.h>

// scdrv
typedef struct {
    long    last_write_time;
    long    last_read_time;

    pid_t   last_write_pid;
    kuid_t   last_write_uid;

    pid_t   last_read_pid;
    kuid_t   last_read_uid;

} scdrv;

// ringbuffer
typedef struct
{
    __u8    *data;  // bytes of data stored in buffer
    int     size; // size of data array
    int     head; // index of reading pointer
    int     tail; // index of writing pointer
} ringbuffer;

static __u8		ringbuffer_read(ringbuffer *buffer);
static void		ringbuffer_write(ringbuffer *buffer, __u8 input);

static int bufsize;
static scdrv 		drv;
static ringbuffer 	buffer;

#endif