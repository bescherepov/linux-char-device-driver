#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc, char* argv[])
{
	FILE* fd = fopen("/dev/scdrv0", "rb");
	char buf[8];

	printf("Reading from /dev/scdrv0...\n");
	size_t rv = fread(&buf, sizeof(char), 8, fd);
	printf("Read result: %s\n", buf);

	fclose(fd);
}