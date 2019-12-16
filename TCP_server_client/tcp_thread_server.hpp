#pragma once

#include <functional>
//#include <thread> //C++线程库
#include <pthread.h>
#include "tcp_socket.hpp"

#define CHECK_RET(exp) if(!(exp)) { return false; }

typedef std::function<void (const std::string&, std::string*)> Handler;

class TcpThreadServer
{
public:
    TcpThreadServer()
    {}

    ~TcpThreadServer()
    {
        listen_sock_.Close();
    }

    bool Start(const std::string& ip, uint16_t port, Handler handler)
    {
        //1、创建socket
        CHECK_RET(listen_sock_.Socket());
        //2、绑定端口号
        CHECK_RET(listen_sock_.Bind(ip, port));
        //3、监听
        CHECK_RET(listen_sock_.Listen());
        //4、进入主循环
        while(true)
        {
            //5、调用accept
            TcpSocket client_sock;
            std::string peer_ip;
            uint16_t peer_port;
            bool ret = listen_sock_.Accept(&client_sock, &peer_ip, &port);
            if(!ret)
            {
                continue;
            }
            printf("[%s:%d]客户端建立连接！\n", peer_ip.c_str(), peer_port);
            //6、创建线程处理客户端逻辑
            ProcessConnect(client_sock, peer_ip, peer_port, handler);
        }
    }
private:
    void ProcessConnect(TcpSocket& client_sock, const std::string& ip, uint16_t port, Handler handler)
    {
        //1、创建线程
        pthread_t tid;
        ThreadEntryArg* arg = new ThreadEntryArg;
        arg->client_sock = client_sock;
        arg->ip = ip;
        arg->port = port;
        arg->handler = handler;
        pthread_create(&tid, NULL, ThreadEntry, (void*)arg);
        //2、主线程，返回
        pthread_detach(tid);
        //3、新线程，循环处理客户端操作
    }
    struct ThreadEntryArg
    {
        TcpSocket client_sock;
        std::string ip;
        uint16_t port;
        Handler handler;
    };
    static void* ThreadEntry(void* arg)
    {
        ThreadEntryArg* argument = (ThreadEntryArg*)arg;
        TcpSocket& client_sock = argument->client_sock;
        std::string& ip = argument->ip;
        uint16_t port = argument->port;
        Handler handler = argument->handler;

        while(true)
        {
            //      a)读取客户端请求
            std::string req;
            int ret = client_sock.Recv(&req);
            if(ret < 0)
            {
                continue;
            }
            if(ret == 0)
            {
                client_sock.Close();
                printf("[%s:%d]客户端断开连接！\n", ip.c_str(), port);
                break;
            }
            //      b)根据请求计算响应
            printf("[%s:%d]客户端请求 %s\n", ip.c_str(), port, req.c_str());
            std::string resp;
            handler(req, &resp);
            //      c)把响应写回客户端
            client_sock.Send(resp);
        }
        delete argument;
    }
private:
    TcpSocket listen_sock_;
};

