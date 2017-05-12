
/*
 * Test file for task 06 of the eudyptula challenge
 * This file will test the read write function of hello_world module
 */

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

#define BUF_LENGTH 13

int main(char * argc, char **argv)
{
        int fd, status, count;
        char content[BUF_LENGTH] = "9850aeb5fe79";
        char rx_buf[BUF_LENGTH];

        fd = open("/dev/eudyptula", O_RDWR);
        if(fd == 0){
                printf("Failed open\n");
                return -EBADF;
        }

        printf("Open Successfully, fd = %d\n", fd);

        //count = write(fd, content, BUF_LENGTH);	
        //if(count == 0)
        //        printf("Failed write\n");

        //printf("Write successfuly, count = %d\n", count);


        count = read(fd, rx_buf, BUF_LENGTH);
        if(count == 0)
                printf("Failed read\n");

        printf("Read successfully, count = %d\n", count);

        return 0;
}
