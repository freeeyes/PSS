#pragma once

//管理所有已建立的连接对象
#include <winsock.h>
#include <Windows.h>
#include <vector>

using namespace std;

#define MAX_BUFF_50    50
#define MAX_BUFF_1024  1024

#define FUNC_SUCCESS          0      //函数执行成功
#define FUNC_FAIL_EXIST       -1     //连接已经存在
#define FUNC_FAIL_NO_EXIST    -2     //连接不存在 
#define FUNC_SEND_ERROR       -3     //发送数据失败

#define INTVALID_SERVERID     -1     //无效的ID

#define TIMER_EVENT_ID        1      //定时器消息ID
#define TIMER_EVENT_INTERVAL  2000   //定时器间隔时间（单位毫秒）
#define TCP_RECV_TIMEOUT      5000   //接收数据超时时间
#define TCP_RECV_SLEEP        1000   //接收数据检查时间

//接收数据的数据格式
class CRecvData
{
public:
	virtual void RecvData(char* pRecvData, int nLen) = 0;
};

//TCP连接对象
struct _TpClientInfo
{
	int        m_nServerID;             //服务器ID
	char       m_szTpIP[MAX_BUFF_50];   //TCP的IP
	int        m_nPort;                 //TCP的端口
	SOCKET     m_sckClient;             //Socket
	int        m_nState;                //0为未连接，1为已连接
	CRecvData* m_pRecvData;             //处理接收数据的回调类

	_TpClientInfo()
	{
		m_nServerID = INTVALID_SERVERID;
		m_szTpIP[0] = '\0';
		m_nPort     = 0;
		m_sckClient = INVALID_SOCKET;
		m_nState    = 0;
		m_pRecvData = NULL;
	}
};

class CTpClientManager
{
public:
	~CTpClientManager(void);

	static CTpClientManager& Instance()
	{
		//初始化TCP链接
		WSADATA wsaData;
		int nErr = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if(nErr != 0)
		{
			//MessageBox(_T("本机socket库加载失败，请检查本机socket库版本"), _T("错误信息"), MB_OK);
			printf_s("本机socket库加载失败，请检查本机socket库版本");
		}

		if(m_pTpClientManager == NULL)
		{
			m_pTpClientManager = new CTpClientManager();
		}

		return *m_pTpClientManager;
	}

	//连接远程服务器
	int TPClient_Connect(int nServerID, const char* pIP, int nPort, CRecvData* pRecvData = NULL);

	//关闭远程服务器连接
	int TPClient_DisConnect(int nServerID);

	//关闭所有连接
	int TPClient_DisConnect_All();

	//发送向指定连接的数据请求
	int TPClient_Send(int nServerID, const char* pBuff, int nLen);

	//获得一个连接的指定状态
	int TPClient_ConnectState(int nServerID);

	//自动重连某一个指定的ServerID
	int TPClient_ReConnect(int nServerID);

	//定时器事件，用于处理定时重连
	int TPClient_HandleEvents(int nEventsID);

	//启动定时器
	int TPClient_Run(bool blIsDebug = false);

	//遍历所有接收数据
	void Recv_All_Data();

	//自动重练所有未连接的客户端
	void Connect_All_Server();

	//是否显示debug输出
	void SetDebug(bool blDebug);

private:
	CTpClientManager(void);
	typedef vector<_TpClientInfo> vecTpClientInfo;

	bool           IsFindTpClient(int nServerID);           //查询指定的ServerID是否存在
	_TpClientInfo* IsGetTpClient(int nServerID);            //找到指定的ServerID
	bool           DelTcpClient(int nServerID);             //删除指定的ServerID
	bool           DelTcpClient_All();                      //删除所有的ServerID

	int            ConnectToServer(_TpClientInfo& objTpClientInfo);    //连接远程服务器

private:
	static CTpClientManager* m_pTpClientManager;
	vecTpClientInfo          m_vecTpClientInfo;
	bool                     m_blDebug;
};
