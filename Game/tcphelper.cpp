#include "tcphelper.h"

TCPHelper::TCPHelper(std::string name, std::string message) {
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer_in[256];

    int portno = 10000;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
        std::cerr << "ERROR opening socket" << std::endl;

    server = gethostbyname(name.c_str());

    if (server == NULL) {
        std::cerr << "ERROR, no such host\n" << std::endl;
        return;
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        std::cerr << "ERROR connecting" << std::endl;

    printf("Please enter the message: ");

    //bzero(buffer, 256);
    //std::cin >> buffer;
    int n = write(sockfd, message.c_str(), message.length());

    if (n < 0)
         std::cerr << "ERROR writing to socket" << std::endl;

    //bzero(buffer,256);
    n = read(sockfd, buffer_in, 255);

    if (n < 0)
         std::cerr << "ERROR reading from socket" << std::endl;

    std::cout << buffer_in << std::endl;

    close(sockfd);

    return;
}
