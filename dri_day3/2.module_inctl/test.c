#include <stdio.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <string.h>
#include "cmd.h"
#include <sys/ioctl.h>

#define N 128

int main(int argc, const char *argv[])
{
	int fd;
	int ret = 0;
	struct cmdbuf buf;
 	struct cmdbuf rbuf;

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
	ioctl(fd,CMD);
	printf("ioctl ok!\n");

	getchar();
	buf.a = 100;
	buf.ch = 'B';
	ioctl(fd,CMDW,&buf);
	printf("ioctl ok!\n");

	getchar();
	ioctl(fd,CMDR,&rbuf);
	printf("ioctl read ok!\n");
	printf("a:%d,ch:%c\n",rbuf.a,rbuf.ch);


	close(fd);
	return 0;
}
