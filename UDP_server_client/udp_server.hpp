#pragma once

#include "udp_socket.hpp"
#include <functional>

typedef std::function<void (const std::string&, std::string* resp)>Handler;

//通用服务器程序
class UdpServer
{
public:
    UdpServer()
    {
        //创建socket文件
        assert(sock_.Socket());
    }

    ~UdpServer()
    {
        //关闭socket文件
        sock_.Close();
    }

    bool Start(const std::string& ip, uint16_t port, Handler handler)
    {
        bool ret = sock_.Bind(ip, port);
        if(!ret)
        {
            return false;
        }
        for(;;)
        {
            std::string req;  //客户端发来的数据
            std::string remote_ip;  //数据来源端的ip地址
            uint16_t remote_port = 0;  //数据来源端的端口号信息
            bool ret = sock_.RecvFrom(&req, &remote_ip, &remote_port);  //三个参数都为输出型参数
            if(!ret)
            {
                continue;
            }
            std::string resp;
            handler(req, &resp);  //通过回调函数对客户端发来的数据进行处理
            sock_.SendTo(resp, remote_ip, remote_port);  //将处理后的数据发回给客户端
            printf("[%s:%d] req:%s, resp:%s\n", remote_ip.c_str(), remote_port, req.c_str(), resp.c_str());
        }
        sock_.Close();
        return true;
    }

private:
    UdpSocket sock_;
};

