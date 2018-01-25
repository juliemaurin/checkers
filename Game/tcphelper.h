#ifndef TCPHELPER_H
#define TCPHELPER_H

// POSIX Socket headers
#include <sys/socket.h> // AF_INET, SOCK_STREAM, socket(), connect()
#include <netdb.h> // getaddrinfo()
#include <unistd.h> // read(), write(), close() (File descriptor operations)

#include <iostream>
using namespace std;
class TCPHelper {
public:
    TCPHelper();
    void openSocket(const string &hostname, const string &port);
    void closeSocket();
    string receive();
    void send(const string &input);

private:
    int sockfd;
};

#endif // TCPHELPER_H
