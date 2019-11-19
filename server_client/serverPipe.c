#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
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
    umask(0);
    if(mkfifo("mypipe", 0664) < 0)
    {
        ERR_EXIT("fifo");
    }
    int rfd = open("mypipe", O_RDONLY);
    if(rfd < 0)
    {
        ERR_EXIT("open");
    }

    char buf[1024];
    while(1)
    {
        buf[0] = 0;
        printf("Please wait...\n");
        ssize_t s = read(rfd, buf, sizeof(buf) - 1);
        if(s < 0)
        {
            ERR_EXIT("read");
        }
        else if(s == 0)
        {
            printf("Client quit, exit now!\n");
            exit(EXIT_SUCCESS);
        }
        else
        {
            buf[s - 1] = 0;
            printf("Client say# %s\n", buf);
        }
    }

    close(rfd);
    return 0;
}

