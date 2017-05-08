#include "UDPServer.h"

UDPServer::UDPServer(std::string IPv4, int port)
{  
    c_IPv4 = new char [IPv4.length()+1];
    std::strcpy (c_IPv4, IPv4.c_str());

    bzero( & serwer, sizeof( serwer ) );
   
    serwer.sin_family = AF_INET;
    serwer.sin_port = htons( port );
    if( inet_pton( AF_INET, c_IPv4, & serwer.sin_addr ) <= 0 )
    {
        std::string error="inet_pton() ERROR";
        throw error;
    }
   
    if(( useSocet = socket( AF_INET, SOCK_DGRAM, 0 ) ) < 0 )
    {
        std::string error="socket() ERROR";
        throw error;
    }
   
    len = sizeof( serwer );
    if( bind( useSocet,( struct sockaddr * ) & serwer, len ) < 0 )
    {
        std::string error="bind() ERROR";
        throw error;
    }
//Default size buffor
    receiptBuffor = new char[receiptBufforSize];
}

UDPServer::~UDPServer()
{
    this->liseningActive=false;
    shutdown( useSocet, SHUT_RDWR );
}

void UDPServer::setReceiptBuffor(int size)
{
    receiptBuffor = new char[size];
    receiptBufforSize = size;
    
}

void UDPServer::sendMessage(std::string targetIPv4, int targetPort, std::string message)
{
    char * c_targetIPv4 = new char [targetIPv4.length()+1];
    std::strcpy (c_targetIPv4, targetIPv4.c_str());

    char * c_message = new char [message.length()+1];
    std::strcpy (c_message, message.c_str());
    
    this->sendMessage(c_targetIPv4, targetPort, c_message);

}

 void UDPServer::sendMessage(char * c_targetIPv4, short targetPort, char * c_message)
 {
    struct sockaddr_in target;
    bzero( & target, sizeof( target ) );

    target.sin_family = AF_INET;
    target.sin_port = htons( targetPort );
    if( inet_pton( AF_INET, c_targetIPv4, & serwer.sin_addr ) <= 0 )
    {
        std::string error="inet_pton() ERROR";
        throw error;
    }
    this->sendMessage(target, c_message, strlen(c_message));
 }

void UDPServer::sendMessage(sockaddr_in &target, char * c_message, size_t messageSize)
{
    if( sendto( useSocet, c_message,messageSize, 0,( struct sockaddr * ) & target, len ) < 0 )
    {
        std::string error="sendto() ERROR IP: ";
        throw error;
    }
}

void UDPServer::startLisening()
{
    struct sockaddr_in from;
    this->liseningActive=true;

    while(this->liseningActive)
    {
        bzero( & from, sizeof( from ) );
        bzero( receiptBuffor, receiptBufforSize );
        ssize_t returnRec = recvfrom( useSocet, receiptBuffor, receiptBufforSize, 0,( struct sockaddr * ) & from, & len );
        if(  returnRec < 0 )
        {
            std::string error = "recvfrom() ERROR: "+ std::to_string(returnRec);
            //exit(-1);
            throw error;
        }
        if(connectManager!=0)
            connectManager->udpReceipt(from,receiptBuffor);
    }   

}

void UDPServer::operationOnReceipt(sockaddr_in from,char *receipt)
{

}
