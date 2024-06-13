#include <fcntl.h>
#include <unistd.h>
int main(int argc, char* argv[])
{
	int fd = open("/dev/scdrv0", S_IRUSR);
	close(fd);
}