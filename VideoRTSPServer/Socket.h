#pragma once
#include <winsock.h>
#include <share.h>
#include <memory>

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

class ESocket {
public:
    ESocket(bool isTcp = true) :m_socket(new Socket(isTcp)) {};
    ESocket(const ESocket& es) :m_socket(new Socket(*es.m_socket)) {};
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

private:
    std::shared_ptr<Socket> m_socket;
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
