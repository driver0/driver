#include <stdio.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/types.h>

int main(int argc, const char *argv[])
{
	int fd;
	
	fd = open("/dev/hello",O_RDWR);
	if(fd < 0)
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
