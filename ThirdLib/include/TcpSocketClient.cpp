/*
 * Source file about portable socket class.
 *
 * design:odison
 * e-mail:odison@126.com>
 *
 */

#include "TcpSocketClient.h"



ODSocket::ODSocket(SOCKET sock)
{
    // m_sock = sock;
}

ODSocket::~ODSocket()
{
}



bool ODSocket::Connect(const char* ip, unsigned short port,int timeout)
{
    remote_addr_(port,hostname);
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting connect to %s:%dn", remote_addr_.get_host_name(),remote_addr_.get_port_number()));

    ACE_Time_Value timeout(timeout,0);

    if (connector_.connect (client_stream_, remote_addr_,&timeout) == -1)
    {
        ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) %pn","connection failed"),-1);
    }
    else
    {
        ACE_DEBUG ((LM_DEBUG,"(%P|%t) connected to %sn", remote_addr_.get_host_name ()));
    }


}

int ODSocket::Send(const char* buf, int len, int flags)
{


    int Send_Count = 0;
    while(true)
    {

        Send_Count = client_stream_.send_n (buf, ACE_OS::strlen(buf)+1);
        if ( Send_Count == -1)
        {
            ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) %pn","send_n"),0);
            break;
        }
    }

    return   Send_Count;

}

int ODSocket::Recv(char* buf, int len, int flags)
{
    //   return (recv(m_sock, buf, len, flags));

    int Recv_Count = 0;
    Recv_Count=client_stream_.recv_n (buf, len );
    if( Recv_Count ==-1)
    {
        ACE_ERROR ((LM_ERROR, "%pn", "Error in recv"));
    }
    else
    {
        // data_buf_[byte_count]=0;
        ACE_DEBUG((LM_DEBUG,"Server received: %sn",buf));
    }

    return Recv_Count;

}

int ODSocket::Close()
{
    if (client_stream_.close () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) %pn","close"),-1);
    else
        return 0;
}



bool ODSocket::DnsParse(const char* domain, char* ip)
{
    struct hostent* p;
    if ( (p = gethostbyname(domain)) == NULL )
        return false;

    sprintf(ip,
            "%u.%u.%u.%u",
            (unsigned char)p->h_addr_list[0][0],
            (unsigned char)p->h_addr_list[0][1],
            (unsigned char)p->h_addr_list[0][2],
            (unsigned char)p->h_addr_list[0][3]);

    return true;
}
