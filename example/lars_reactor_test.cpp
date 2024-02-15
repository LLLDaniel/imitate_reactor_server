#include "../include/tcp_server.h"

int main() {
        TcpServer tcp_server("127.0.0.1", 8080);
        tcp_server.DoAccept();
        return 0;
}
