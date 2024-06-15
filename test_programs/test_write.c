#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	FILE* fd = fopen("/dev/scdrv0", "wb");
	char buf[4] = "abc";

	printf("Writing to /dev/scdrv0 %s...\n", buf);
	size_t rv = fwrite(&buf, sizeof(char), 4, fd);
	printf("Check /dev/scdrv0, abc must have been written.\n");

	fclose(fd);
}