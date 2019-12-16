//#include "tcp_server.hpp"
//#include "tcp_process_server.hpp"
#include "tcp_thread_server.hpp"
#include <iostream>
#include <unordered_map>

int main()
{
    std::unordered_map<std::string, std::string> dict;
    dict.insert(std::make_pair("hello", "你好"));
    dict.insert(std::make_pair("world", "世界"));
    dict.insert(std::make_pair("i", "我"));
    dict.insert(std::make_pair("love", "爱"));
    dict.insert(std::make_pair("you", "你"));
    dict.insert(std::make_pair("really", "真地"));
    TcpThreadServer server;
    server.Start("0.0.0.0", 9090, [&dict](const std::string& req, std::string* resp){
                 auto it = dict.find(req);
                     if(it == dict.end())
                     {
                        *resp = "未找到！";
                     }
                     else
                     {
                        *resp = it->second;
                     }
                 });
    return 0;
}

