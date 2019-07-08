#include <iostream>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
using namespace std;

int main()
{
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork");
        exit(1);
    }
    else if(pid == 0)
    {
        sleep(3);
        exit(10);
    }
    else
    {
        int st;
        int ret = wait(&st);

        if(ret > 0 && (st & 0x7f) == 0)
        {
            cout << "child exit code:" << ((st >> 8) & 0xff) << endl;
        }
        else if(ret > 0)
        {
            cout << "sig code:" << (st & 0x7f) << endl;
        }
    }
    return 0;
}

