/* 自主minishell实现
*		1、获取标准输入
*		2、解析输入得到[命令名] + [运行参数]
*		3、创建子进程
*		    子进程中进行程序替换
*		4、进程等待
*/
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>


char buf[1024] = {0};
int do_face()
{
//获取字符串
    memset(buf, 0x00, 1024);
    printf("[uesr@localhost]$ ");
    fflush(stdout);
    //%[^\n]  从缓冲区取数据的时候，遇到\n为止[      ls    -l]
    //经过这一步后，缓冲区中留下了\n，但是\n取不出来，导致scanf非阻塞
    //但又取不出最后的\n字符
    //%*c      从缓冲区中取出一个字符（丢弃）
    if(scanf("%[^\n]%*c", buf) != 1)  //scanf给几个空间分配内容(赋值)就返回几
    {
        //若在输入状态多按几次空格
        //解析失败，缓冲区中还存在一个回车字符未取出来
        getchar();
        return -1;
    }
    // printf("buf:[%s]\n", buf);
    return 0;
}
int argc = 0;
char* argv[32];
int do_parse()
{
    argc = 0;
    char* ptr = buf;
    while(*ptr != '\0')
    {
        //将指针走到非空白字符处
        if(!isspace(*ptr))  //不是空白字符
        {
            argv[argc] = ptr;
            argc++;
            //将ls走完，认为ls是一个完整的字符串
            //不能仅仅将l字符串作为第0个参数
            while(!isspace(*ptr) && *ptr != '\0')
            {
                ptr++;
            }
            //在ls之后添加一个字符串结尾标志
            //因为我们想获取的是ls，而不是[ls    -l     ]
            *ptr = '\0';
            // printf("argv[%d] = %s\n", argc, argv[argc - 1]);
        }
        ptr++;
    }
    //argv[] = {"ls", "-l", NULL,}
    argv[argc] = NULL;
    return 0;
}
int main()
{
	while(1)
	{
        //获取标准输入，显示shell提示
        if(do_face() < 0)
        {
            continue;
        }
        //对字符串进行解析，去掉空格，将字符串解析成字符串指针数组
        if(do_parse() < 0)
        {
            continue;
        }

//实现shell内建命令
		if(strcmp(argv[0], "cd") == 0)
		{
			//int chdir(const char* path);
			//改变当前工作路径
			chdir(argv[1]);
			continue;
		}

//实现shell命令
		int pid = fork();
		if(pid < 0)
		{
			perror("fork error");
			return -1;
		}
		else if(pid == 0)
		{
            int i;
            int fd = 1;
            for(i = 0; i < argc; ++i)
            {
                if(strcmp(argv[argc], ">") == 0)
                {
                    fd = open(argv[argc + 1], O_WRONLY | O_CREAT | O_TRUNC);
                    dup2(fd, 1);
                }
                else if(strcmp(argv[argc + 1], ">>") == 0)
                {
                    fd = open(argv[argc + 1], O_WRONLY | O_CREAT | O_APPEND);
                    dup2(fd, 1);
                }
			//进程替换
			execvp(argv[0], argv);
			exit(-1);
    		}
		//进程等待--避免僵尸进程
		wait(NULL);

    	}
	return 0;
    }
}
