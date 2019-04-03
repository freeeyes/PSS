#ifndef _TESTCASE_H
#define _TESTCASE_H

#include <vector>
#include <thread> 
#include <mutex>
#include <stdio.h>  
#include <stdlib.h>  

#include <string.h>  
#include <errno.h>  
#include <assert.h>  
#include <time.h>

//#include "pthread.h"


#include "commons.h"
#include "sock_wrap.h"




//²âÊÔµ¥¸öÊý¾Ý°üÊÕ·¢Çé¿ö
bool CheckTcpPacket(_ClientInfo& objClientInfo, _ResultInfo& objResultInfo);

//²âÊÔ¶à¸öÊý¾Ý°üÊÕ·¢Çé¿ö
bool CheckMultipleTcpPacket(int nCount, _ClientInfo& objClientInfo, _ResultInfo& objResultInfo);

//²âÊÔ¶à¸öÁ¬½ÓÊÕ·¢°üÇé¿ö
bool CheckMultipleTcpConnect(int nCount, _ClientInfo& objClientInfo, _ResultInfo& objResultInfo);

//²âÊÔ·Ç·¨Êý¾Ý°üÍ·µÄÎÊÌâ
bool CheckTcpErrorPacketHead(_ClientInfo& objClientInfo, _ResultInfo& objResultInfo);

//²âÊÔÊý¾ÝÕ³°ü
bool CheckTcpHalfPacket(_ClientInfo& objClientInfo, _ResultInfo& objResultInfo);

//¶àÏß³Ì²âÊÔÁ¬½Ó
bool Thread_CheckTcpPacket(_ClientInfo& objClientInfo, _ResultInfo& objResultInfo);

//²âÊÔ¶àÏß³Ì²¢·¢
bool CheckTcpMulipleThreadPacket(int nCount, _ClientInfo& objClientInfo, _ResultInfo& objResultInfo);

//²âÊÔUDP°ü·¢ËÍ
bool Thread_CheckUdpPacket(_ClientInfo& objClientInfo, _ResultInfo& objResultInfo, _ResultInfo& objRecvResultInfo);

//²âÊÔUDP°ü½ÓÊÕ
bool Thread_CheckUdpPacket_Recv(_ClientInfo& objClientInfo, _ResultInfo& objResultInfo);

//²âÊÔConsoleÁ¬½Ó
bool CheckConsolePacket(_ResultInfo& objResultInfo);

#endif
