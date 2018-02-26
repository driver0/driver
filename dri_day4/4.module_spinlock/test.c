#include <stdio.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <string.h>

#define N 128

int main(int argc, const char *argv[])
{
	int fd;
	int ret = 0;

	fd = open("/dev/hello",O_RDWR);
	if(fd<0)
	{
		perror("Failed to open.");
		return -1;
	}
	else
	{
		printf("Open success.\n");
	}

	getchar();

	close(fd);
	return 0;
}
