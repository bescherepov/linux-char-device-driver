#include "structs.h"
#include "scdrv_io.h"
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/module.h>

const char 	*device_name = "scdrv";
unsigned int major;
unsigned int minor = 1;

struct cdev scdrv_cdev;
dev_t 		dev;

MODULE_AUTHOR("Bogdan Bescherepov");
MODULE_DESCRIPTION("Simple char driver (scdrv)");
MODULE_LICENSE("GPL v2");
MODULE_PARM_DESC(bufsize, "Parameter used to set char driver buffer size");
module_param(bufsize, int, S_IRUGO); // buffer size parameter

int init_module(void); // driver.c
void cleanup_module(void); // driver.c