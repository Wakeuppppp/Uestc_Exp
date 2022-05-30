#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class CTcpServer {
public:
    int m_listenfd;
    int m_clientfd;

    CTcpServer();

    bool InitServer(int port);

    bool Accept();


    int Send(const void *buf, const int buflen);

    int Recv(void *buf, const int buflen);

    ~CTcpServer();
};

int main() {
    CTcpServer TcpServer;
    printf("这是一个服务器\n");
    printf("等待连接...\n");
    if (TcpServer.InitServer(5051) == false) {
        printf("TcpServer.InitServer(5051) failed,exit...\n");
        return -1;
    }

    if (TcpServer.Accept() == false) {
        printf("TcpServer.Accept() failed,exit...\n");
        return -1;
    }

    printf("客户端已连接。\n");

    char strbuffer[1024];

    while (1) {
        memset(strbuffer, 0, sizeof(strbuffer));
        if (TcpServer.Recv(strbuffer, sizeof(strbuffer)) <= 0) break;
        printf("接收：%s\n", strbuffer);
    }

    printf("客户端已断开连接。\n");
}

CTcpServer::CTcpServer() {

    m_listenfd = m_clientfd = 0;
}

CTcpServer::~CTcpServer() {
    if (m_listenfd != 0) close(m_listenfd);
    if (m_clientfd != 0) close(m_clientfd);
}

bool CTcpServer::InitServer(int port) {
    m_listenfd = socket(AF_INET, SOCK_STREAM, 0);


    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    if (bind(m_listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) != 0) {
        close(m_listenfd);
        m_listenfd = 0;
        return false;
    }


    if (listen(m_listenfd, 5) != 0) {
        close(m_listenfd);
        m_listenfd = 0;
        return false;
    }

    return true;
}

bool CTcpServer::Accept() {
    if ((m_clientfd = accept(m_listenfd, 0, 0)) <= 0) return false;

    return true;
}

int CTcpServer::Send(const void *buf, const int buflen) {
    return send(m_clientfd, buf, buflen, 0);
}

int CTcpServer::Recv(void *buf, const int buflen) {
    return recv(m_clientfd, buf, buflen, 0);
}
