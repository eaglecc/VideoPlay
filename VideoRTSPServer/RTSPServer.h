#pragma once
#include "Socket.h"
#include <string>
#include "HeThread.h"
#include <map>


class RTSPRequest
{
public:
    RTSPRequest() {}
    RTSPRequest(const RTSPRequest& rtsp);
    RTSPRequest& operator=(const RTSPRequest& rtsp);
    ~RTSPRequest() {}
private:
    int m_method; // 0: OPTIONS 1: DESCRIBE 2: SETUP 3: PLAY 4: TEARDOWN
};

class RTSPReply {
public:
    RTSPReply() {}
    RTSPReply(const RTSPReply& reply);
    RTSPReply& operator=(const RTSPReply& reply);
    ~RTSPReply() {}
private:
    int m_method; // 0: OPTIONS 1: DESCRIBE 2: SETUP 3: PLAY 4: TEARDOWN
};

class RTSPSession {
public:
    RTSPSession() {}
    RTSPSession(const RTSPSession& session);
    RTSPSession& operator=(const RTSPSession& session);
    ~RTSPSession() {}
};

class RTSPServer : public ThreadFuncBase
{
public:
    RTSPServer() :m_socket(true) {}
    ~RTSPServer() {}

    int Init(const std::string& strIp = "0.0.0.0", short port = 554);
    int Invoke();
    void Stop();

protected:
    int ThreadWorker();
    RTSPRequest AnanlyseReauest(const std::string& request);
    RTSPReply MakeReply(const RTSPRequest& req);
    int ThreadSession();

private:
    ESocket m_socket;
    int m_status; // 0: 未初始化 1：初始化完成 2：运行中 3：关闭
    HeThread m_threadMain;
    HeThreadPool m_pool;
    std::map<std::string, RTSPSession> m_mapSessions;
    static SocketIniter m_initer;
};
