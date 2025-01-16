#pragma once
#include "Socket.h"
#include <string>

class RTSPServer
{
public:
    RTSPServer() :m_socket(true) {}
    ~RTSPServer() {}

    int Init(const std::string& strIp = "0.0.0.0", short port = 554);
    int Invoke();
    void Stop();

private:
    ESocket m_socket;
    int m_status; // 0: 未初始化 1：初始化完成 2：运行中 3：关闭

};

