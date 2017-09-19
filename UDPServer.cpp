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
   
    if(( usedSocet = socket( AF_INET, SOCK_DGRAM, 0 ) ) < 0 )
    {
        std::string error="socket() ERROR";
        throw error;
    }
   
    len = sizeof( serwer );
    if( bind( usedSocet,( struct sockaddr * ) & serwer, len ) < 0 )
    {
        std::string error="bind() ERROR";
        throw error;
    }

    messageBuffor = new char[messageBufforSize];

    //fcntl(usedSocet, F_SETFL, O_NONBLOCK);
}

UDPServer::~UDPServer()
{
    for(int i = Receipts.size(); i; i--)
    {
        delete Receipts.back();
        Receipts.pop_back();
    }

    shutdown( usedSocet, SHUT_RDWR );
}

void UDPServer::sendMessage(sockaddr_in &target, char * c_message, size_t messageSize)
{
    if( sendto( usedSocet, c_message,messageSize, 0,( struct sockaddr * ) & target, len ) < 0 )
    {
        std::string error="sendto() ERROR IP: ";
        throw error;
    }
}

int UDPServer::getReceipt()
{
    FD_ZERO(&fd_in);
    FD_SET(usedSocet, &fd_in);
    
    struct timeval waitTime;
    waitTime.tv_sec=0;
    waitTime.tv_usec=0;

    int rv = select(usedSocet + 1, &fd_in, NULL, NULL, &waitTime);
    if(rv<=0) 
        return rv;

    struct sockaddr_in * from = new sockaddr_in;
    bzero( & from, sizeof( from ) );
    bzero( messageBuffor, messageBufforSize );
    ssize_t returnRec = recvfrom( usedSocet, messageBuffor, messageBufforSize, 0,( struct sockaddr * ) & from, & len );
    if(  returnRec < 0 )
    {
        delete from;
        std::string error = "recvfrom() ERROR: "+ std::to_string(returnRec);
        return -2;
    } 

    Receipt *receipt = new Receipt;
    receipt->message= new char[messageBufforSize];
    memcpy(receipt->message, messageBuffor, messageBufforSize);
    
    //strncpy(receipt->message, messageBuffor, messageBufforSize);
    receipt->from = from;
    Receipts.push_back(receipt);
    //receiptBuffor->addReceipt( &from, messageBuffor);
    return 1;

}
