#pragma once

#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include <cassert>
#include <string>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

//对socketAPI进行封装
class UdpSocket
{
public:
    UdpSocket()
        :fd_(-1)
    {}

    //创建socket文件
    bool Socket()
    {
        //创建socket文件，返回文件描述符
        //第一个参数：表示创建的套接字协议族，决定了socket的地址类型，通信中必须采用对应的地址，AF_INET表示ipv4地址
        //第二个参数：表示socket类型，如SOCK_STREAM一般应用于TCP协议，SOCK_DGRAM一般应用于UDP协议
        //第三个参数：表示指定的协议
        fd_ = socket(AF_INET, SOCK_DGRAM, 0);
        if(fd_ < 0)
        {
            perror("socket");
            return false;
        }
        return true;
    }

    bool Close()
    {
        close(fd_);
        return true;
    }

    //绑定端口号：把一个socket文件个一个端口号关联起来，一个进程可以关联多个端口号，但是一个端口号只能关联一个进程
    //通过socket调用创建的套接字必须经过命名（绑定地址）后才能使用
    //bind系统调用把address中的地址分配给描述符socket关联的未命名套接字，地址长度由addr_len决定
    bool Bind(const std::string& ip, uint16_t port)
    {
        sockaddr_in addr;  //表示待绑定的地址，结构体指针，指向要绑定给sockfd的协议地址，这个地址根据创建socket时第地址协议族的不同而不同
        addr.sin_family = AF_INET;  //ipv4地址格式
        addr.sin_addr.s_addr = inet_addr(ip.c_str());  //接受传进来的ip地址的客户连接
        addr.sin_port = htons(port);  //port为服务器端指定的端口号

        int ret = bind(fd_, (sockaddr *)&addr, sizeof(addr));
        if(ret < 0)
        {
            perror("bind");
            return false;
        }
        return true;
    }

    //接收数据
    bool RecvFrom(std::string* buf, std::string* ip = nullptr, uint16_t* port = nullptr)
    {
        char tmp[1024 * 10] = {0};
        sockaddr_in peer;
        socklen_t len = sizeof(peer);
        //用该函数从TCP连接的对端接收数据
        //第一个参数指定接收端套接字描述符
        //第二个参数指明一个缓冲区，该缓冲区用来存放接收的数据
        //第三个参数指明缓冲区buf的长度
        //第四个参数一般置为0
        //第五个参数是一个结构体类型变量，保存源机器(数据来源端)的IP地址和端口号
        //第六个参数是结构体类型变量大小
        ssize_t read_size = recvfrom(fd_, tmp, sizeof(tmp) - 1, 0, (sockaddr*)&peer, &len);
        if(read_size < 0)
        {
            perror("recvfrom");
            return false;
        }

       //将读到的缓冲区内容放到输出函数中 
       //*buf = tmp;
       buf->assign(tmp, read_size);
       if(ip)
       {
           *ip = inet_ntoa(peer.sin_addr);
       }
       if(port)
       {
           *port = ntohs(peer.sin_port);
       }

       return true;
    }

    //发送数据
    bool SendTo(const std::string& buf, const std::string& ip, uint16_t port)
    {
        sockaddr_in addr;

        //第一个参数指定发送端套接字描述符
        //第二个参数指明 一个存放应用程序要发送数据的缓冲区
        //第三个参数指明实际要发送的字节数
        //第四个参数一般置为0
        //第五个参数表示目的机的ip地址和端口号
        //第六个参数表示长度
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        addr.sin_port = htons(port);
        ssize_t write_size = sendto(fd_, buf.data(), buf.size(), 0, (sockaddr*)&addr, sizeof(addr));
        if(write_size < 0)
        {
            perror("sendto");
            return false;
        }
        return true;
    }
private:
    int fd_;  //socket文件描述符
};

