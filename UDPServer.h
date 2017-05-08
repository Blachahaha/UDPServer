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

#include <iostream>
#include <cstring>
#include <string>

class UDPServer
{
    public:
    UDPServer(std::string IPv4, int port);
    ~UDPServer();
    void setReceiptBuffor(int size);

    void sendMessage(std::string targetIPv4, int targetPort, std::string message);
    void sendMessage(char * c_targetIPv4, short targetPort, char * c_message);
    void sendMessage(sockaddr_in &target, char * c_message, size_t messsageSize);
    
    void startLisening();

    virtual void operationOnReceipt(sockaddr_in from,char *receipt);



    
    private:
    char * receiptBuffor;
    bool liseningActive=false;
    char *c_IPv4;
    struct sockaddr_in serwer;
    socklen_t len;
    int useSocet;
    char *buffor;

    int receiptBufforSize=512;

};

#endif 
