#include <iostream>
#include <jsoncpp/json/json.h>
#include "tcp_thread_server.hpp"

//使用JSON的方式进行序列化和反序列化
//{
//  "num1": 10,
//  "num2": 20,
//  "op": "+"
//}
//
//{
//  "result": 30
//}
//jsoncpp
//Json::Value  用来表示一个Json数据
//Json::Reader 把字符串转换成Json::Value
//Json::Writer 把Json::Value转成字符串

int main()
{
    TcpThreadServer server;
    server.Start("0.0.0.0", 9090, [](const std::string& req, std::string* resp){
                    //1、将req反序列化
                    Json::Reader reader;
                    Json::Value req_json;
                    reader.parse(req, req_json);
                    //根据反序列化结果，计算
                    int num1 = req_json["num1"].asInt();
                    int num2 = req_json["num2"].asInt();
                    std::string op = req_json["op"].asString();
                    int result = 0;
                    if(op == "+")
                    {
                       result = num1 + num2;
                    }
                    else if(op == "-")
                    {
                       result = num1 - num2;
                    }
                    else if(op == "*")
                    {
                       result = num1 * num2;
                    }
                    else if(op == "/")
                    {
                        result = num1 / num2;
                    }
                    //3、把结果序列化回一个字符串
                    Json::FastWriter writer;
                    Json::Value resp_json;
                    resp_json["result"] = result;
                    *resp = writer.write(resp_json);
                 });
    return 0;
}

