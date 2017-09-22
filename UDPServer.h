#ifndef _UDPServer_h_
#define _UDPServer_h_

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>

#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <cstring>
#include <string>
#include <list>

struct Receipt
{
    char *message;
    sockaddr_in *from;
};


class UDPServer
{
    public:
    UDPServer(std::string IPv4, int port);
    ~UDPServer();
    void sendMessage(sockaddr_in *target, char * c_message, size_t messsageSize);
    int getReceipt(); 

    char * messageBuffor;
    std::list < Receipt* > Receipts;

    private:
    char *c_IPv4;
    struct sockaddr_in serwer;
    socklen_t len;
    int usedSocet;
    char *buffor;
    int messageBufforSize=256;
    fd_set fd_in;
};

#endif 