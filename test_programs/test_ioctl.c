#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <sys/types.h>

#define 	SCDRV_IOCTL_NUM 0x11

#define     SCDRV_IOCTL_SET_IO_BLOCKING 				_IOW(SCDRV_IOCTL_NUM, 1, bool)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_TIME 	_IOR(SCDRV_IOCTL_NUM, 2, long int)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_PID 	_IOR(SCDRV_IOCTL_NUM, 3, int)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_TIME 	_IOR(SCDRV_IOCTL_NUM, 4, long int)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_PID 	_IOR(SCDRV_IOCTL_NUM, 5, int)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_READ_UID 	_IOR(SCDRV_IOCTL_NUM, 6, int)
#define     SCDRV_IOCTL_BUFFER_ACCESS_LAST_WRITE_UID 	_IOR(SCDRV_IOCTL_NUM, 7, int)

int main(int argc, char* argv[])
{
	int fd = open("/dev/scdrv0", O_RDONLY);
	if (fd == -1){
		printf("scdrv not opened");
		return -1;
	}
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
	close(fd);

}