
/*
 * Test file for task 08 of the eudyptula challenge
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

        fd = open("/sys/kernel/debug/eudyptula/id", O_RDWR);
        if(fd == 0){
                printf("Failed open\n");
                return -EBADF;
        }

        printf("Open Successfully, fd = %d\n", fd);

        count = write(fd, content, BUF_LENGTH);
        if(count == 0)
                printf("Failed write\n");

        printf("Write successfuly, count = %d\n", count);

        count = read(fd, rx_buf, BUF_LENGTH);
        if(count == 0)
                printf("Failed read");

        printf("Read successfully, count = %d\n", count);

        int fd_jiffies;
        unsigned long jiffies;

        fd_jiffies = open("/sys/kernel/debug/eudyptula/jiffies", O_RDONLY);
        if(fd == 0){
                printf("Failed open jiffies\n");
                return -EBADF;
        }

        printf("open successfully jiffies, fd_jiffies = %d\n", fd_jiffies);

        count = read(fd_jiffies, &jiffies, sizeof(jiffies));
        if(count == 0)
                printf("failed read jiffies\n");

        printf("Read successfully, jiffies = %ld\n", jiffies);

        int fd_foo;
        unsigned long r_value;
        unsigned long w_value = 1;
        fd_foo = open("/sys/kernel/debug/eudyptula/foo", O_RDWR);
        if(fd == 0){
                printf("Failed open foo\n");
                return -EBADF;
        }

        printf("open successfully, fd_foo = %d", fd_foo);

        count = write(fd_foo, &w_value, sizeof(w_value));
        if(count == 0)
                printf("Failed write foo\n");

        printf("Write successfully foo");

        count = read(fd_foo, &r_value, sizeof(r_value));
        if(count == 0)
                printf("Failed read foo\n");

        printf("successfully read foo, foo = %ld", r_value);

        return 0;
}
