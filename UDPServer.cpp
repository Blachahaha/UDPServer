#include "UDPServer.h"

UDPServer::UDPServer(std::string IPv4, int port)
{  
    char bufor[256];
    c_IPv4 = new char [IPv4.length()+1];
    std::strcpy (c_IPv4, IPv4.c_str());

    bzero( & serwer, sizeof( serwer ) );
    bzero( bufor, sizeof( bufor ) );
   
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
    buffor = new char[8];
}

UDPServer::~UDPServer()
{
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
        /*error+=c_targetIPv4;
        error+=" Port: ";
        error+=std::to_string(targetPort);*/
        throw error;
    }
}

void UDPServer::startLising()
{
    struct sockaddr_in from;

    while(1)
    {
        bzero( & from, sizeof( from ) );

        if( recvfrom( useSocet, receiptBuffor, 256, 0,( struct sockaddr * ) & from, & len ) < 0 )
        {
            std::string error = "recvfrom() ERROR";
            throw error;
        }
       this->operationOnReceipt(from,receiptBuffor);
    }    

}

/*void UDPServer::test()
{
    while( 1 )
    {
        struct sockaddr_in from;
        bzero( & from, sizeof( from ) );
        bzero( bufor, sizeof( bufor ) );
       
        printf( "Waiting for connection...\n" );
        if( recvfrom( useSocet, bufor, sizeof( bufor ), 0,( struct sockaddr * ) & from, & len ) < 0 )
        {
            perror( "recvfrom() ERROR" );
            exit( - 1 );
        }
        printf( "|Wiadomosc od clienta|: %s \n", bufor );
        char bufor_ip[ 128 ];
        bzero( bufor_ip, sizeof( bufor_ip ) );
        printf( "|Client ip: %s port: %d|\n", inet_ntop( AF_INET, & from.sin_addr, bufor_ip, sizeof( bufor_ip ) ), ntohs( from.sin_port ) );
        //printf("  New connection from: %s:%d\n",inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
       
        bzero( bufor, sizeof( bufor ) );
        strcpy( bufor, "Wyslane z serwera" );
        if( sendto( useSocet, bufor, strlen( bufor ), 0,( struct sockaddr * ) & from, len ) < 0 )
        {
            perror( "sendto() ERROR" );
            exit( - 1 );
        }
    }
}*/

void UDPServer::operationOnReceipt(sockaddr_in from,char *receipt)
{
    
}