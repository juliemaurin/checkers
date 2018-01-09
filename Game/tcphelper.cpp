#include "tcphelper.h"

TCPHelper::TCPHelper(const std::string &hostname, const std::string &port, const std::string &input) {
    struct addrinfo hints = {};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo *server;

    if (getaddrinfo(hostname.c_str(), port.c_str(), &hints, &server)) {
        throw std::runtime_error("TCPHelper : No such host");
    }

    int sockfd = socket(server->ai_family, server->ai_socktype, server->ai_protocol);
    if (sockfd < 0) {
        throw std::runtime_error("TCPHelper : Failed to open socket");
    }

    if (connect(sockfd, server->ai_addr, server->ai_addrlen) < 0) {
        throw std::runtime_error("TCPHelper : Failed to connect to host");
    }

    if (write(sockfd, input.c_str(), input.length()) < 0) {
        throw std::runtime_error("TCPHelper : Failed to write to socket");
    }

    const int obuf_size = 256;
    char obuf[obuf_size];
    if (read(sockfd, obuf, obuf_size) < 0) {
        throw std::runtime_error("TCPHelper : Failed to read from socket");
    }

    close(sockfd);

    freeaddrinfo(server);

    std::cout << obuf << std::endl;
}
