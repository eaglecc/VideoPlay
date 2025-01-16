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
    int m_status; // 0: δ��ʼ�� 1����ʼ����� 2�������� 3���ر�

};

