#include "GLOBAL.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define POLICY_LEN 10
// User function
void GetTime(int pid)
{
    clock_t t;
    t = clock();
    printf("[Project1] ");
    printf("pid:%d ", pid);
    printf("time: %lu\n",t);
}
void Pause(int goal)
{
    for(goal;goal>0;goal--){
            for(waiter=0;waiter<1000000UL;waiter++);
    }
}
int callSystemTime(pid_t pid, int status) {
	char buf[32];
 	int fd;
 
    if ((fd = open("/dev/osproj", O_RDONLY)) < 0){
        perror("failed to open /dev/osproj");
        return 0;
 	}
	sprintf(buf,"%ld", (long)pid); 

    //strcpy(buf, pid);
    ioctl(fd, status, buf);

    close(fd);
    return 1;
}
