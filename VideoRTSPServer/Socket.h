#pragma once
#include <winsock.h>
#include <share.h>
#include <memory>

/*自定义类继承string，额外封装一些运算符重载和功能*/
class HBuffer :public std::string {
public:
    HBuffer(const char* str) {
        resize(strlen(str));
        memcpy((void*)c_str(), str, size());
    }
    HBuffer(size_t size = 0) :std::string() {
        if (size > 0) {
            resize(size);
            memset(*this, 0, this->size());
        }
    }
    HBuffer(void* buffer, size_t size) :std::string() {
        resize(size);
        memcpy((void*)c_str(), buffer, size);
    }
    ~HBuffer() {
        std::string::~basic_string();
    }

    operator char* () const { return (char*)c_str(); }
    operator const char* () const { return c_str(); }
    operator BYTE* () const { return (BYTE*)c_str(); }
    operator void* () const { return (void*)c_str(); }

    void Update(void* buffer, size_t size) {
        resize(size);
        memcpy((void*)c_str(), buffer, size);
    }

    void Zero() {
        if (size() > 0) memset((void*)c_str(), 0, size());
    }

    HBuffer& operator<<(const HBuffer& str) {
        if (this != str) {
            *this += str;
        }
        else {
            HBuffer tmp = str;
            *this += tmp;
        }
        return *this;
    }

    HBuffer& operator<<(const std::string& str) {
        *this += str;
        return *this;
    }

    HBuffer& operator<<(const char* str) {
        *this += HBuffer(str);
        return *this;
    }

    HBuffer& operator<<(int data) {
        char s[16] = "";
        snprintf(s, sizeof(s), "%d", data);
        *this += s;
        return *this;
    }

    const HBuffer& operator>>(int& data) const {
        data = atoi(c_str());
        return *this;
    }

    const HBuffer& operator>>(short& data) const {
        data = (short)atoi(c_str());
        return *this;
    }
};

class Socket
{
public:
    // nType：true表示TCP，false表示UDP
    Socket(bool nType = true) {
        m_socket = INVALID_SOCKET;
        if (nType) {
            m_socket = socket(AF_INET, SOCK_STREAM, 0);
        }
        else {
            m_socket = socket(AF_INET, SOCK_DGRAM, 0);
        }
    }

    Socket(SOCKET s) {
        m_socket = s;
    }

    void Close() {
        if (m_socket != INVALID_SOCKET) {
            closesocket(m_socket);
            m_socket = INVALID_SOCKET;
        }
    }
    operator SOCKET() {
        return m_socket;
    }

    ~Socket() {
        Close();
    }
private:
    SOCKET m_socket;
};

class EAddress {
public:
    EAddress() {
        m_port = -1;
        memset(&m_addr, 0, sizeof(m_addr));
        m_addr.sin_family = AF_INET;
    }
    EAddress(const EAddress& addr) {
        m_ip = addr.m_ip;
        m_port = addr.m_port;
        m_addr = addr.m_addr;
    }
    EAddress& operator=(const EAddress& addr) {
        if (this != &addr) {
            m_ip = addr.m_ip;
            m_port = addr.m_port;
            m_addr = addr.m_addr;
        }
        return *this;
    }
    ~EAddress() {}
    operator const sockaddr* () const {
        return (sockaddr*)&m_addr;
    }
    operator sockaddr_in* () {
        return &m_addr;
    }
    operator sockaddr* () {
        return (sockaddr*)&m_addr;
    }

    void Update(const std::string& ip, short port) {
        m_ip = ip;
        m_port = port;
        m_addr.sin_port = htons(port);
        m_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    }
    int size() const {
        return sizeof(sockaddr_in);
    }

private:
    std::string m_ip;
    short m_port;
    sockaddr_in m_addr;
};


class ESocket {
public:
    ESocket(bool isTcp = true) :m_socket(new Socket(isTcp)), m_istcp(isTcp) {};
    ESocket(const ESocket& es) :m_socket(new Socket(*es.m_socket)), m_istcp(es.m_istcp) {};
    ESocket(SOCKET sock, bool isTcp = true) :m_socket(new Socket(sock)), m_istcp(isTcp) {};
    ESocket& operator=(const ESocket& sock) {
        if (this != &sock) {
            m_socket = sock.m_socket;
        }
        return *this;
    }
    ~ESocket() {
        m_socket.reset();
    }
    operator SOCKET() {
        return *m_socket;
    }

    int Bind(const EAddress& addr) {
        if (m_socket == nullptr)
        {
            m_socket.reset(new Socket(m_istcp));
        }
        return bind(*m_socket, addr, addr.size());
    }

    int Listen(int backlog = 5) {
        return listen(*m_socket, backlog);
    }

    int Connect(const EAddress& addr) {
        return connect(*m_socket, addr, addr.size());
    }

    void Close() {
        m_socket.reset();
    }

    ESocket Accept(EAddress& addr) {
        int len = addr.size();
        auto s = accept(*m_socket, addr, &len);
        return ESocket(s, m_istcp);
    }

    int Recv(HBuffer& buffer) {
        return recv(*m_socket, buffer, buffer.size(), 0);
    }

    int Send(const HBuffer& buffer) {
        return send(*m_socket, buffer, buffer.size(), 0);
    }

private:
    std::shared_ptr<Socket> m_socket;
    bool m_istcp;
};

class SocketIniter {
public:
    SocketIniter() {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    }
    ~SocketIniter() {
        WSACleanup();
    }
};
