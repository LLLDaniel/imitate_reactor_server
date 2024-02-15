#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

class TcpServer {
    public:
        TcpServer(const char *ip, uint16_t port);
        ~TcpServer();

        void DoAccept();

    private:
        int sockfd_;
        struct sockaddr_in connaddr_;
        socklen_t addrlen_;
};
