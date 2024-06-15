#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <time.h>

#define     SCDRV_IOCTL_SET_IO_BLOCKING 				1
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_TIME 	2
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_PID 	3
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_TIME 	4
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_PID 	5
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_UID 	6
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_UID 	7

int main(int argc, char* argv[])
{
	int fd = open("/dev/scdrv0", S_IRUSR);
	struct timespec read_time;

	ioctl(fd, SCDRV_IOCTL_SET_IO_BLOCKING, false);
	printf("Check dmesg. SCDRV log about blocking mode must have been created.\n");
	ioctl(fd, SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_TIME, &read_time);
	printf("Last scdrv read time: %ld.%ld\n", read_time.tv_sec, read_time.tv_nsec);

}