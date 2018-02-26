#ifndef __CMD_HEAD_H
#define __CMD_HEAD_H

struct cmdbuf{
	int a;
	char ch;
};

#define CMD 	_IO('c',0)
#define CMDR 	_IOR('c',1,struct cmdbuf)
#define CMDW 	_IOW('c',2,struct cmdbuf)

#endif
