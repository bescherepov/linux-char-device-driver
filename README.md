# Quite simple char device driver with ring buffer
A character device driver that allows information to be transmitted between two independent processes using the ring buffer principle.

The size of the ring buffer is the kernel module parameter.

It is possible to write and read to the buffer of the character driver. By default, these operations are set in blocking mode, but it is possible to set unblocking mode (see ioctl calls).

Operations can be tracked using dmesg.

I am newbie in writing Linux kernel modules and I don't pretend this code to be professional and production ready. Opposite, despite I tested main operations and it seems to work right, actions accessing the buffer may be insecure, and the some of specific functionality may work not as it must. But I consider this code will make you easier to understand basics of Linux kernel module creating.

Also there is much system logging in project because it is in debug mode by default and I just didn't take on removing it. For production version it is should be reduced.

ioctl calls:
1. Setting the blocking/non-blocking read/write mode
2. Getting the time (in seconds from OS start) of the last buffer read operation
3. Getting the ID of the process that performed the last read operation
4. Getting the time (in seconds from OS start) of the last buffer read operation
5. Getting the ID of the process that performed the last write operation
6. Getting the ID of the owner of the process that performed the last read operation
7. Getting the ID of the owner of the process that performed the last write operation

## Source code files
`scdrv.h` - description of the module

`buffer.h` - structure of the ring buffer

`scdrv_io.h` - file operations such as read, write, open, release, ioctl

## Build and install
`./install.sh `

The script will ask you to enter the size of the driver buffer. After that, you can request the superuser password to install and configure the module.

## Input/output via the console

`echo abc > /dev/scdrv0`

`cat /dev/scdrv0`

## Driver check
`ls /dev/scdrv0`

`dmesg` (lines "SCDRV:") 

Logging of almost every driver operation is done only for debugging, in practice it should be significantly reduced.

## Testing
The test_programs folder contains the programs test_read.c, test_ioctl.c, test_write.c to check the driver's operability

`gcc test_programs/test_write.c -o write`

`gcc test_programs/test_read.c -o read`

`gcc test_programs/test_ioctl.c -o ioctl`

`./test_programs/write`

`./test_programs/read`

`./test_programs/ioctl`
