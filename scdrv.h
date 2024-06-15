#ifndef SCDRV
#define SCDRV

#include "buffer.h" // ringbuffer
#include "scdrv_io.h" // fops, ioctl
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/time.h>

#define SUCCESS 			0
#define SCDRV_NAME 			"SCDRV"
#define SCDRV_CLASS_NAME 	"scdrv_class"
#define SCDRV_DEVICE_NAME 	"scdrv0"

MODULE_AUTHOR("Bogdan Bescherepov");
MODULE_DESCRIPTION("Simple char driver (scdrv)");
MODULE_LICENSE("GPL v2");
MODULE_PARM_DESC(bufsize, "Parameter used to set char driver buffer size");
module_param(bufsize, int, S_IRUGO); // buffer size parameter

extern int 	init_module(void);
extern void cleanup_module(void);

unsigned int major;
unsigned int minor;

struct cdev 		scdrv_cdev;
struct class 	*scdrv_cdev_class;
dev_t 			dev;

int bufsize;
ringbuffer scdrv_buf;

#endif