#include <stdio.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <string.h>

#define N 128

int main(int argc, const char *argv[])
{
	int fd;
	char rbuf[N] = {};
	char wbuf[N] = {};
	int ret = 0;

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

	strcpy(wbuf,"This is a write test.");
	ret = write(fd,wbuf,strlen(wbuf));
	printf("write %d bytes.\n",ret);

	strcpy(wbuf,"update ++++++.");
	ret = write(fd,wbuf,strlen(wbuf));
	printf("write %d bytes.\n",ret);
	
	getchar();
	ret = read(fd,rbuf,5);
	printf("read %d bytes : %s\n",ret,rbuf);
	memset(rbuf,'\0',N);

	getchar();
	ret = read(fd,rbuf,10);
	printf("read %d bytes : %s\n",ret,rbuf);
	memset(rbuf,'\0',N);

	getchar();
	ret = read(fd,rbuf,N);
	printf("read %d bytes : %s\n",ret,rbuf);
	memset(rbuf,'\0',N);

	close(fd);
	return 0;
}
