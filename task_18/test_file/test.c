#include "stdio.h"
#include "fcntl.h"
#include "string.h"
#include "unistd.h"

int main(void)
{
	int fd, ret;
	char *write_str = "Alice";
	char *write_str2 = "Bob";
	char *write_str3 = "Dave";
	char *write_str4 = "Gena";
	
	fd = open("/dev/eudyptula", O_RDWR);
	if (fd < 0)
		printf("fd = %d\n", fd);
	
	ret = write(fd, write_str, strlen(write_str) + 1);
	if (ret)
		printf("ret = %d\n", ret);

	ret = write(fd, write_str2, strlen(write_str2) + 1);
	if (ret)
		printf("ret = %d\n", ret);
	
	ret = write(fd, write_str3, strlen(write_str3) + 1);
	if (ret)
		printf("ret = %d\n", ret);
	
	ret = write(fd, write_str4, strlen(write_str4) + 1);
	if (ret)
		printf("ret = %d\n", ret);		
	
	printf("seeya\n");
	return 0;
}

