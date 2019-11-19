#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define ERR_EXIT(m) \
    do{\
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

int main()
{
    int wfd = open("mypipe", O_WRONLY);
    if(wfd < 0)
    {
        ERR_EXIT("open");
    }
    char buf[1024];
    while(1)
    {
        buf[0] = 0;
        printf("Please Enter# ");
        fflush(stdout);
        ssize_t s = read(0, buf, sizeof(buf) - 1);
        if(s <= 0)
        {
            ERR_EXIT("read");
        }
        else
        {
            buf[s] = 0;
            write(wfd, buf, strlen(buf));
        }
    }

    close(wfd);
    return 0;
}

