#ifndef TCPHELPER_H
#define TCPHELPER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#include <iostream>
#include <cstring>

class TCPHelper
{
public:
    TCPHelper(std::string name, std::string message);
};

#endif // TCPHELPER_H
