#pragma once
#include "Socket.h"
#include <string>
#include "HeThread.h"
#include <map>
#include "CHeQueue.h"

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
    HBuffer toBuffer();
private:
    int m_method; // 0: OPTIONS 1: DESCRIBE 2: SETUP 3: PLAY 4: TEARDOWN
};

class RTSPSession {
public:
    RTSPSession() {}
    RTSPSession(const ESocket& client);
    RTSPSession(const RTSPSession& session);
    RTSPSession& operator=(const RTSPSession& session);
    ~RTSPSession() {}

    RTSPRequest AnanlyseReauest(const std::string& request);
    RTSPReply MakeReply(const RTSPRequest& req);
private:
    std::string m_id;
    ESocket m_client;
};


class RTSPServer : public ThreadFuncBase
{
public:
    RTSPServer() :m_socket(true), m_status(0), m_pool(10) {
        // 将 RTSPServer 类的 threadWorker 成员函数设置为主线程的工作函数
        m_threadMain.UpdateWorker(ThreadWorker(this, (FUNCTYPE)&RTSPServer::threadWorker));
    }
    ~RTSPServer();

    int Init(const std::string& strIp = "0.0.0.0", short port = 554);
    int Invoke();
    void Stop();

protected:
    int threadWorker(); // 0：继续,负数：终止,其他：警告
    int ThreadSession();

private:
    ESocket m_socket;
    EAddress m_addr;
    int m_status; // 0: 未初始化 1：初始化完成 2：运行中 3：关闭
    HeThread m_threadMain;
    HeThreadPool m_pool;
    std::map<std::string, RTSPSession> m_mapSessions;
    static SocketIniter m_initer;
    CHeQueue<RTSPSession> m_listSessions;
};
