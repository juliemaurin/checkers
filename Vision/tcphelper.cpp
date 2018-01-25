#include "tcphelper.h"

TCPHelper::TCPHelper() : sockfd(-1), client_sock(-1) {

}

TCPHelper::TCPHelper(const string &hostname, const string &port) : sockfd(-1), client_sock(-1) {
    openSocket(hostname, port);
}

void TCPHelper::send(const string &input) {
    if (write(client_sock, input.c_str(), input.length()) < 0) {
        throw runtime_error("TCPHelper : Failed to write to socket");
    }  
}

string TCPHelper::receive(){
    const int ibuf_size = 256;
    char ibuf[ibuf_size];

    if (read(client_sock, ibuf, ibuf_size) < 0) {
        throw runtime_error("TCPHelper : Failed to read from socket");
    }

    cout << "Received Data :"<< ibuf << endl;

    return string(ibuf);
}

void TCPHelper::connect() {
    sockaddr_in client_addr;
    socklen_t sin_size = sizeof(client_addr);

    client_sock = accept(sockfd, reinterpret_cast<sockaddr*>(&client_addr), &sin_size);

    if (client_sock == -1) {
        throw runtime_error("Could not accept");
    }
}

void TCPHelper::disconnect() {
    if (client_sock != -1) close(client_sock);
    client_sock = -1;
}

void TCPHelper::openSocket(const string &hostname, const string &port){
    addrinfo hints = {};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    addrinfo *server;

    if (getaddrinfo(hostname.c_str(), port.c_str(), &hints, &server)) {
        throw runtime_error("TCPHelper : No such host");
    }

    sockfd = socket(server->ai_family, server->ai_socktype, server->ai_protocol);
    if (sockfd < 0) {
        throw runtime_error("TCPHelper : Failed to open socket");
    }

    if(bind(sockfd, server->ai_addr, server->ai_addrlen)) {
      throw runtime_error("TCPHelper : Failed to bind server socket to server address ");
	}

    if(listen(sockfd, 5)) {
        throw runtime_error("TCPHelper : Failed to listen socket");
    }
}

void TCPHelper::closeSocket() {
    if (client_sock != -1) close(client_sock);
    client_sock = -1;
    close(sockfd);
}
