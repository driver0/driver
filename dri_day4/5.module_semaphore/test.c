#include <stdio.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#define N 128

int main(int argc, const char *argv[])
{
	int fd;
	int ret = 0;
	pid_t pid;
	char buf[N] = {};

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

	if((pid = fork()) < 0)
	{
		perror("Failed ro fork");
		return -1;
	}
	else if(pid == 0)
	{
		if(read(fd,buf,N) < 0)
		{
			perror("Failed to read.");
		}
		printf("read:%s\n",buf);
	}
	else   //parent
	{
		sleep(5);
		strcpy(buf,"this is a semaphore test.");
		write(fd,buf,strlen(buf)+1);
		printf("Write ok!\n");
	}


	close(fd);
	return 0;
}
