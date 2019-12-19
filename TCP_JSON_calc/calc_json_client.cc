#include <iostream>
#include <jsoncpp/json/json.h>
#include "tcp_client.hpp"

int main()
{
    //1、创建一个TCP客户端
    TcpClient client;
    int ret = client.Connect("127.0.0.1", 9090);
    if(!ret)
    {
        printf("connect failed!\n");
        return 1;
    }
    //2、循环从标准输入读入数据 
    while(true)
    {
        printf("请输入后缀表达式的运算式子：");
        fflush(stdout);
        int num1, num2;
        char op[10] = {0};
        scanf("%d %d %s", &num1, &num2, op);
        //3、根据用户输入的内容，构造成一个JSON对象
        Json::Value req_json;
        req_json["num1"] = num1;
        req_json["num2"] = num2;
        req_json["op"] = op;
        //4、把JSON对象序列化成一个字符串
        Json::FastWriter writer;
        std::string req = writer.write(req_json);
        printf("%s\n", req.c_str());
        //5、把字符串发送到服务器端
        client.Send(req);
        //6、从服务器读取返回的结果
        std::string resp;
        //7、把服务器返回的结果再进行JSON解析
        Json::Reader reader;
        Json::Value resp_json;
        reader.parse(resp, resp_json);
        printf("result: %d\n", resp_json["result"].asInt());
    }
    return 0;
}

