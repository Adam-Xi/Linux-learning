#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;


//编程规范约定：
//一个函数的输入参数用 const &
//一个函数的输出参数用 *
//一个函数的输入输出参数用 &


//关于字符串的切分
//strtok用起来较为鸡肋，麻烦，且不安全
//其他方法：
//C++标准库的做法
//1、#include <sstream>  //stringstream
//基于boost的方案是比较科学的方案
//2、boost algorithm/string.hpp    boost::split
//
int Split(char input[], char* output[])
{
    //char* strtok(char* str, const char* delim)
    //执行过程
    //破坏原始的字符串
    //在内部记录上次的切分结果(static)
    //必须循环调用，并且第一次的参数和后续的参数不一样
    char* p = strtok(input, " ");
    int i = 0;
    while(p != NULL)
    {
        //cout << p << endl;
        output[i] = p;
        i++;
        p = strtok(NULL, " ");
    }
    output[i] = NULL;       //切记，exec函数族若要使用，需使传入数组的最后一项为NULL
    return i;
}

void CreateProcess(char* argv[], int n)
{
    //创建子进程
    pid_t id = fork();
    if(id > 0)
    {
        //father
        wait(NULL);
    }
    else if(id == 0)
    {
        //child
        int ret = execvp(argv[0], argv);
        //if对返回值的判断条件可以省略，如果exec成功，是不会执行到的
        perror("exec");
        exit(0);
    }
    else
    {
        //error
    }
    //父进程进行进程等待，子进程进行程序替换
}

int main()
{
    while(1)
    {
        //打印提示符
        cout << "[myshell@localhost ~]$";
        fflush(stdout);

        //用户 输入一个指令
        char command[1024] = {0};
        gets(command);
        //cout << command << endl;

        //解析指令，把要执行的指令和命令行参数识别出来，字符串切分
        //切分结果是一个字符串数组，strtok
        char* argv[1024] = {0};
        int n = Split(command, argv);

        //4、创建子进程，进程替换
        CreateProcess(argv, n);
    }
    return 0;
}

