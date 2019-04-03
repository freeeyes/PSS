#ifndef _CLIENTDEFINE_H
#define _CLIENTDEFINE_H

#define MESSAGE_SENDERROR        "发送给服务器命令失败"
#define MESSAGE_TITLE_ERROR      "错误信息"
#define MESSAGE_TITLE_SUCCESS    "服务器信息" 
#define MESSAGE_TITLE_CONNECT    "设置服务器地址成功"
#define MESSAGE_RESULT_SUCCESS   "命令执行成功"
#define MESSAGE_RESULT_FAIL      "命令执行失败"
#define MESSAGE_INSERT_NULL      "必须输入相关信息才能进行此操作"
#define MESSAGE_TITLE_INFO       "提示信息"
#define MESSAGE_IS_CLOSE_SERVER  "是否真的关闭服务器？"
#define MESSAGE_IS_CLOSE_OVER    "服务器关闭指令已发出"

struct _ClientConnectInfo
{
  char       m_szIP[50];            //IP地址信息
  int        m_nConnectID;          //链接的ID
  int        m_nRecvCount;          //接收包数量
  int        m_nSendCount;          //发送包数量
  int        m_nAllRecvSize;        //接收字节数
  int        m_nAllSendSize;        //发送字节数
  int        m_nBeginTime;          //链接建立时间
  int        m_nAliveTime;          //存活时间秒数
  int        m_nRecvQueueCount;     //接受逻辑处理包的个数
  long long  m_nRecvQueueTimeCost;  //接受逻辑处理包总时间消耗
  long long  m_nSendQueueTimeCost;  //发送数据总时间消耗

  _ClientConnectInfo()
  {
    m_szIP[0]            = '\0';
    m_nConnectID         = 0;
    m_nRecvCount         = 0;
    m_nSendCount         = 0;
    m_nAllRecvSize       = 0;
    m_nAllSendSize       = 0;
    m_nBeginTime         = 0;
    m_nAliveTime         = 0;
    m_nRecvQueueCount    = 0;
    m_nRecvQueueTimeCost = 0;
    m_nSendQueueTimeCost = 0;
  }
};


#endif
