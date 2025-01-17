#include "RTSPServer.h"

RTSPServer::~RTSPServer()
{
    Stop();
}

int RTSPServer::Init(const std::string& strIp, short port)
{
    m_addr.Update(strIp, port);
    m_socket.Bind(m_addr);
    m_socket.Listen();

    return 0;
}

int RTSPServer::Invoke()
{
    m_threadMain.Start();
    return 0;
}

void RTSPServer::Stop()
{
    m_socket.Close();
    m_threadMain.Stop();
    m_pool.Stop();
}

int RTSPServer::threadWorker()
{
    EAddress client_addr;
    ESocket client = m_socket.Accept(client_addr);
    if (client != INVALID_SOCKET)
    {
        m_clients.PushBack(client);
        m_pool.DispatchWorker(ThreadWorker(this, (FUNCTYPE)&RTSPServer::ThreadSession));
    }
    return 0;
}

int RTSPServer::ThreadSession()
{
    ESocket client;
    HBuffer buffer(1024 * 16);
    int len = client.Recv(buffer);
    if (len <= 0) {
        return -1;
    }
    buffer.resize(len);
    RTSPRequest req = AnanlyseReauest(buffer);
    RTSPReply ack = MakeReply(req);
    client.Send(ack.toBuffer());

    return 0;
}
