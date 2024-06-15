#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <time.h>
#include <sys/types.h>

#define     SCDRV_IOCTL_SET_IO_BLOCKING 				1
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_TIME 	2
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_PID 	3
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_TIME 	4
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_PID 	5
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_UID 	6
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_UID 	7

int main(int argc, char* argv[])
{
	int fd = open("/dev/scdrv0", S_IRUSR | S_IWUSR);
	long int read_time;
	long int write_time;
	pid_t   last_write_pid;
	uid_t   last_write_uid;
	pid_t   last_read_pid;
	uid_t   last_read_uid;

	ioctl(fd, SCDRV_IOCTL_SET_IO_BLOCKING, 0);
	printf("Check dmesg. SCDRV log about blocking mode must have been created.\n");

	ioctl(fd, SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_TIME, &read_time);
	printf("Last scdrv read time: %ld\n", read_time);

	ioctl(fd, SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_TIME, &write_time);
	printf("Last scdrv write time: %ld\n", write_time);

	ioctl(fd, SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_PID, &last_read_pid);
	printf("Last scdrv read pid: %d\n", last_read_pid);

	ioctl(fd, SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_PID, &last_write_pid);
	printf("Last scdrv write pid: %d\n", last_write_pid);

	ioctl(fd, SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_UID, &last_read_uid);
	printf("Last scdrv read uid: %d\n", last_read_uid);

	ioctl(fd, SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_UID, &last_write_uid);
	printf("Last scdrv write uid: %d\n", last_write_uid);

}