#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#define SYS_EUDYPTULA 333

int main(void)
{
	int ret;
	long id_combined;
	uint32_t high_id;
	uint32_t low_id;
	char id[13] = "9850aeb5fe79";
	char *ptr;

	id_combined = strtol(id, &ptr, 16);

	printf("id_combined = %lu\n", id_combined);
	
	high_id = id_combined & 0xFFFFFFFF00000000;
	low_id = id_combined & 0x00000000FFFFFFFF;

	printf("high_id = %d\n", high_id);
	printf("low_id = %d\n", low_id);
	
	ret = syscall(SYS_EUDYPTULA, high_id, low_id);
	if (ret)
		printf("Could not call sys_eduyptula, ret = %d\n", ret);

	return ret;
}
