#ifndef _SOCK_WRAP_H_
#define _SOCK_WRAP_H_

//#include "platform.h"


#if defined(_WIN32_PLATFORM_)

#include <winsock2.h>
#include <windows.h>
#define timelong_t ULARGE_INTEGER
typedef SOCKET HSocket;
#endif

#if defined(_LINUX_PLATFORM_)
#include <sys/time.h>
#include <linux/errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#define timelong_t struct timeval


typedef int HSocket;
#define SOCKET_ERROR  (-1)
#define INVALID_SOCKET  0
#endif

#include <thread>

class CMyTimeSpan
{
public:
    CMyTimeSpan();
    void Reset();
    unsigned long long GetSpaninMicroseconds();
    unsigned int GetSpaninMilliseconds();
    unsigned int GetSpaninSeconds();

private:
    timelong_t m_start;
    void getCurrentTimeLong(timelong_t* tl);

};
//=====================================================================================



#if defined(_LINUX_PLATFORM_)
unsigned long GetTickCount();
#endif

typedef struct
{
    int block;
    int sendbuffersize;
    int recvbuffersize;
    int lingertimeout;
    int recvtimeout;
    int sendtimeout;
} socketoption_t;

typedef struct
{
    int nbytes;
    int nresult;
} transresult_t;

int InitializeSocketEnvironment();
void FreeSocketEnvironment();
void GetAddressFrom(sockaddr_in* addr, const char* ip, int port);
void GetIpAddress(char* ip, sockaddr_in* addr);
bool IsValidSocketHandle(HSocket handle);
int GetLastSocketError();

HSocket SocketOpen(int tcpudp);
void SocketClose(HSocket& handle);

int SocketBlock(HSocket hs, bool bblock);
int SocketTimeOut(HSocket hs, int recvtimeout, int sendtimeout, int lingertimeout);

int GetSocketName(HSocket hs,sockaddr_in* addr);
int GetPeerName(HSocket hs,sockaddr_in* addr);

int SocketBind(HSocket hs, sockaddr_in* addr);
int SocketConnect(HSocket hs, sockaddr_in* paddr);
HSocket SocketAccept(HSocket hs, sockaddr_in* addr);
int SocketListen(HSocket hs, int maxconn);

void SocketSend(HSocket hs, const char* ptr, int nbytes, transresult_t& rt);
void SocketRecv(HSocket hs, char* ptr, int nbytes, transresult_t& rt);
void SocketTryRecv(HSocket hs, char* ptr, int nbytes, int milliseconds, transresult_t& rt);
void SocketTrySend(HSocket hs, const char* ptr, int nbytes, int milliseconds, transresult_t& rt);


int SockSendTo (HSocket hs, const char* ptr, int len, const struct sockaddr_in* to);

int SockRecvFrom(HSocket hs, const char* ptr, int len, struct sockaddr_in* from);


void SocketClearRecvBuffer(HSocket hs);

class CSockWrap
{
public:
    CSockWrap(int tcpudp);
    ~CSockWrap();
    void SetAddress(const char* ip, int port);
    void SetAddress(sockaddr_in* addr);
    int SetTimeOut(int recvtimeout, int sendtimeout, int lingertimeout);
    int SetBufferSize(int recvbuffersize, int sendbuffersize);
    int SetBlock(bool bblock);

    HSocket  GetHandle ()
    {
        return m_hSocket;
    }
    void Reopen(bool bForceClose);
    void Close();
    transresult_t Send(void* ptr, int nbytes);
    transresult_t Recv(void* ptr, int nbytes );
    transresult_t TrySend(void* ptr, int nbytes, int milliseconds);
    transresult_t TryRecv(void* ptr, int nbytes, int  milliseconds );
    void ClearRecvBuffer();

protected:
    HSocket  m_hSocket;
    sockaddr_in m_stAddr;
    int m_tcpudp;
};


#endif
