#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int i = 0;
    char bar[102];
    memset(bar, 0, sizeof(bar));
    const char * lable = "|/-\\";
    while(i <= 100)
    {
        printf("[%-100s][%d%%][%c]\r", bar, i, lable[i % 4]);
        fflush(stdout);
        bar[i++] = '#';
        usleep(50000);
    }
    printf("\n");
    printf("waiting");
    for(i = 0; i < 3; i++)
    {
        printf(".\r");
        usleep(1000000);
    }
    printf("\nloading engine...\n");

    
    return 0;
}
