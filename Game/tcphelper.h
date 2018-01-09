#ifndef TCPHELPER_H
#define TCPHELPER_H

// POSIX Socket headers
#include <sys/socket.h> // AF_INET, SOCK_STREAM, socket(), connect()
#include <netdb.h> // getaddrinfo()
#include <unistd.h> // read(), write(), close() (File descriptor operations)

#include <iostream>

class TCPHelper {
public:
    TCPHelper(const std::string &name, const std::string &port, const std::string &input);
};

#endif // TCPHELPER_H
