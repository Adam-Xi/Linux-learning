#include <sstream>
#include "tcp_thread_server.hpp"

int main()
{
    TcpThreadServer server;
    server.Start("0.0.0.0", 9090, [](const std::string& req, std::string* resp){
                    //1、反序列化
                    //  a)strtok()线程不安全问题
                    //  b)find(), isspace()
                    //  c)stringstream
                    std::stringstream ss(req);
                    int num1 = 0;
                    int num2 = 0;
                    std::string op;
                    ss >> num1;
                    ss >> num2;
                    ss >> op;
                    printf("req: %d, %d, %s\n", num1, num2, op.c_str());
                    //2、计算
                    int ret = 0;
                    if(op == "+")
                    {
                        ret = num1 + num2;
                    }
                    else
                    {
                        ret = num1 - num2;
                    }
                    //3、把结果序列化
                    std::stringstream ss_resp;
                    ss_resp << ret;
                    *resp = ss_resp.str();
                 });
    return 0;
}

