
  
#ifndef _ODSOCKET_H_  
#define _ODSOCKET_H_  
  
#include "ace/SOCK_Connector.h"
#include "ace/INET_Addr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"

#define SIZE_BUF 128
#define NO_ITERATIONS 1




class ODSocket {  
  
public:  
    ODSocket(SOCKET sock = INVALID_SOCKET);  
    ~ODSocket();  
  
    // Connect socket  
    bool Connect(const char* ip, unsigned short port,int timeout = 0);  
      
    // Send socket  
    int Send(const char* buf, int len, int flags = 0);  
  
    // Recv socket  
    int Recv(char* buf, int len, int flags = 0);  
      
    // Close socket  
    int Close();  
  

    // Domain parse  
    static bool DnsParse(const char* domain, char* ip);  
  

protected:  
    ACE_SOCK_Stream client_stream_;
     ACE_INET_Addr remote_addr_;
     ACE_SOCK_Connector connector_;
  
};  
#endif
