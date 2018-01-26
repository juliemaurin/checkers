#include "tcphelper.h"

TCPHelper::TCPHelper() {
}

void TCPHelper::send(const string &input){
    if (write(sockfd, input.c_str(), input.length()) < 0) {
        throw runtime_error("TCPHelper : Failed to write to socket");
    }
}

string TCPHelper::receive(){
    const int obuf_size = 256;
    char obuf[obuf_size];
    if (read(sockfd, obuf, obuf_size) < 0) {
        throw runtime_error("TCPHelper : Failed to read from socket");
    }

    return std::string(obuf);
}

void TCPHelper::openSocket(const string &hostname, const string &port){
  struct addrinfo hints = {};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo *server;

    if (getaddrinfo(hostname.c_str(), port.c_str(), &hints, &server)) {
        throw runtime_error("TCPHelper : No such host");
    }

    sockfd = socket(server->ai_family, server->ai_socktype, server->ai_protocol);
    if (sockfd < 0) {
        throw runtime_error("TCPHelper : Failed to open socket");
    }

    if (connect(sockfd, server->ai_addr, server->ai_addrlen) < 0) {
        throw runtime_error("TCPHelper : Failed to connect to host");
    }
}

void TCPHelper::closeSocket(){
    close(sockfd);
}
