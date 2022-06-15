#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>

using namespace std;

class CTcpClient {
public:
    int m_sockfd;

    CTcpClient();

    bool ConnectToServer(const char *serverip, const int port);

    int Send(const void *buf, const int buflen);

    int Recv(void *buf, const int buflen);

    ~CTcpClient();
};

int main() {
    CTcpClient TcpClient;
    printf("这是一个客户端\n");
    printf("连接服务器...\n");


    if (TcpClient.ConnectToServer("39.99.119.232", 5051) == false) {
        printf("TcpClient.ConnectToServer(\"39.99.119.232\",5051) failed,exit...\n");
        return -1;
    }
    char strbuffer[1024];
    char str[1024];
    while (1) {
        cin.getline(str, 1024);
        if(str[0] == 'q') break;
        memset(strbuffer, 0, sizeof(strbuffer));
        sprintf(strbuffer, str);
        TcpClient.Send(strbuffer, strlen(strbuffer));
        printf("发送：%s\n", strbuffer);

    }
}

CTcpClient::CTcpClient() {
    m_sockfd = 0;
}

CTcpClient::~CTcpClient() {
    if (m_sockfd != 0) close(m_sockfd);
}

bool CTcpClient::ConnectToServer(const char *serverip, const int port) {
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct hostent *h;
    if ((h = gethostbyname(serverip)) == 0) {
        close(m_sockfd);
        m_sockfd = 0;
        return false;
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    memcpy(&servaddr.sin_addr, h->h_addr, h->h_length);

    if (connect(m_sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) != 0) {
        close(m_sockfd);
        m_sockfd = 0;
        return false;
    }

    return true;
}

int CTcpClient::Send(const void *buf, const int buflen) {
    return send(m_sockfd, buf, buflen, 0);
}

int CTcpClient::Recv(void *buf, const int buflen) {
    return recv(m_sockfd, buf, buflen, 0);
}
