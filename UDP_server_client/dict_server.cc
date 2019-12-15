//英译汉服务器
#include "udp_server.hpp"
#include <unordered_map>
#include <iostream>

std::unordered_map<std::string, std::string> g_dict;

void Translate(const std::string& req, std::string* resp)
{
    auto it = g_dict.find(req);
    if(it == g_dict.end())
    {
        *resp = "未找到!";
        return ;
    }
    *resp = it->second;
}


int main()
{
    //初始化服务器数据
    g_dict.insert(std::make_pair("hello", "你好"));
    g_dict.insert(std::make_pair("world", "世界"));
    g_dict.insert(std::make_pair("C++", "世界上最好的编程语言"));
    g_dict.insert(std::make_pair("i", "我"));
    g_dict.insert(std::make_pair("love", "爱,喜欢"));
    g_dict.insert(std::make_pair("you", "你"));

    //启动服务器
    UdpServer server;

    server.Start("0.0.0.0", 22, Translate);

    return 0;
}

