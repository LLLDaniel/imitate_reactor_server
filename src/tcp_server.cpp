#include "tcp_server.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

TcpServer::TcpServer(const char *ip, uint16_t port) {
        if (signal(SIGHUP, SIG_IGN) == SIG_ERR) {
                std::cerr << "signal ignore SIGHUP failed.";
        }
        if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
                std::cerr << "signal ignore SIGPIPE failed.";
        }
        sockfd_ = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, IPPROTO_TCP);
        if (sockfd_ == -1) {
                std::cerr << "create tcp server failed.";
                exit(-1);
        }
        struct sockaddr_in server_addr;
        bzero(&server_addr, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        inet_aton(ip, &server_addr.sin_addr);
        server_addr.sin_port = htons(port);

        int opt = 1;
        if (setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
                std::cerr << "set socket to reuse failed." << std::endl;
        }

        if (bind(sockfd_, (const struct sockaddr *)&server_addr,
                 sizeof(server_addr)) < 0) {
                std::cerr << "bind sockfd failed." << std::endl;
                exit(-1);
        }

        if (listen(sockfd_, 1024) < 0) {
                std::cerr << "listen sockfd failed." << std::endl;
                exit(-1);
        }
}
TcpServer::~TcpServer() { close(sockfd_); }

void TcpServer::DoAccept() {
        int connfd;
        while (true) {
                connfd =
                    accept(sockfd_, (struct sockaddr *)&connaddr_, &addrlen_);
                if (connfd == -1) {
                        if (errno == EINTR) {
                                std::cerr << "accept error EINTR." << std::endl;
                                continue;
                        } else if (errno == EAGAIN) {
                                std::cerr << "accept error EAGAIN."
                                          << std::endl;
                                break;
                        } else if (errno == EMFILE) {
                                std::cerr << "accept error EMFILE."
                                          << std::endl;
                                continue;
                        } else {
                                std::cerr << "accet error." << std::endl;
                        }
                } else {
                        std::cout << "get a new connection." << std::endl;
                        int writed;
                        const char *data = "hello imitate reactor server.";
                        do {
                                writed = write(connfd, data, strlen(data) + 1);
                        } while (writed == -1 && errno == EINTR);
                        if (writed > 0) {
                                std::cout << "write success." << std::endl;
                        }
                }
        }
}
