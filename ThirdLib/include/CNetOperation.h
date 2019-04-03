/***************************************************************
 * Name:      CNetOperation.h
 * Purpose:   Defines Application Frame
 * Author:    smith ()
 * Created:   2019-02-21
 * Copyright: smith ()
 * License:
 **************************************************************/

#ifndef CNETOPERATION_H
#define CNETOPERATION_H


#include <cstring>
#include <vector>
#include <iostream>  
#include <fstream>  
#include <sstream>  
#include <iomanip>
#include <string>



    
#include "macro.h"
#include "sock_wrap.h"



using namespace std;


#define MAX_BUFF_20 20

#define MAX_BUFF_50 50
#define MAX_BUFF_500 500
#define MAX_BUFF_10240 10240
#define MAX_BUFF_200 200


#define COMMAND_FTP_LOGIN 0x2000                //登陆
#define COMMAND_FTP_LOGOUT 0x2001               //退出
#define COMMAND_FTP_FILELIST 0x2002             //文件，文件夹列表
#define COMMAND_FTP_FILE_DOWNLOAD 0x2003        //文件下载
#define COMMAND_FTP_FILE_UPLOAD 0x2004          //文件上传
#define COMMAND_FTP_RETURN_LOGIN 0xe000         //登陆应答
#define COMMAND_FTP_RETURN_LOGOUT 0xe001        //登出应答
#define COMMAND_FTP_RETURN_FILELIST 0xe002      //文件，文件夹列表应答
#define COMMAND_FTP_RETURN_FILE_DOWNLOAD 0xe003 //文件下载
#define COMMAND_FTP_RETURN_FILE_UPLOAD 0xe004   //文件上传


#define COMMAND_LOGINCLIENT_LOGIN         0x2100     //登陆
#define COMMAND_LOGINCLIENT_LOGOUT        0x2101     //退出
#define COMMAND_LOGINCLIENT_USERINFO      0x2102     //获得玩家信息
#define COMMAND_LOGINCLIENT_SET_USERINFO  0x2103     //设置用户信息




#define LOGIN_SUCCESS 0
#define LOGIN_FAIL_NOEXIST 1
#define LOGIN_FAIL_ONLINE 2
#define LOGIN_FAIL_PASSWORD 3

#define OP_OK 0
#define OP_FAIL 1

#define IS_FILE 0
#define IS_DIRRNT 1

struct FileInfo {
    std::string FileName;
    int FileType;
    int FileSize;
};

struct Cmd_Packet_Info {
    short CmdVersion;                      //命令版本号
    short CmdID;                           //命令编码
    int CmdPayloadLen;                     //为实际载荷长度
    char CmdSession[32] = { '\0' };        // Session名称
    char CmdBuff[MAX_BUFF_500] = { '\0' }; //用于存储序列化和反序列化后的字节流Buff
    int CmdBuffPos = 0;
    //以上为头部信息
    Cmd_Packet_Info()
    {
        CmdVersion = 1;
        CmdID = 0x2000;
        CmdPayloadLen = 0;
        // CmdSession =;
    }

    auto SetBuffValue(const char* ParamDesc, const int ParamSize, const char* ParamSrc)
    {
        memcpy_s((void*)ParamDesc, ParamSize, ParamSrc, ParamSize);
        CmdBuffPos += ParamSize;
        
    }
/*
    auto GetBuffValue(const char* ParamDesc, const int ParamSize, const char* ParamSrc)
    {
        memcpy_s(ParamDesc, ParamSize, ParamSrc, ParamSize);
        CmdBuffPos += ParamSize;    
    }
 */
   
    template<typename ParamType=char>
    auto SetCmdPayload(const std::string& ParamValue)
    {
        int ParamLength =(int)ParamValue.length();
        SetBuffValue(&CmdBuff[CmdBuffPos], sizeof(ParamType), reinterpret_cast<const char*>(&ParamLength));
        SetBuffValue(&CmdBuff[CmdBuffPos], ParamLength, reinterpret_cast<const char*>(ParamValue.c_str()));
        CmdPayloadLen   =CmdPayloadLen+sizeof(ParamType)+ParamLength;     
    }   
 
    auto SetCmdPayload(const int &ParamLength,const char* ParamValue)
    {
        SetBuffValue(&CmdBuff[CmdBuffPos], ParamLength, ParamValue);
        CmdPayloadLen   =CmdPayloadLen+ParamLength;
    }    
    
    template <typename T = int> auto SetCmdPayload(const T& ParamValue)
    {
        SetBuffValue(&CmdBuff[CmdBuffPos], sizeof(ParamValue), reinterpret_cast<const char*>(&ParamValue));
        CmdPayloadLen   =CmdPayloadLen+sizeof(ParamValue);
     }    
    
    auto SetCmdHead()
    {
        
        CmdBuffPos = 0;
  
        SetBuffValue(&CmdBuff[CmdBuffPos], sizeof(short), reinterpret_cast<const char*>(&CmdVersion));
        SetBuffValue(&CmdBuff[CmdBuffPos], sizeof(short), reinterpret_cast<const char*>(&CmdID));
        SetBuffValue(&CmdBuff[CmdBuffPos], sizeof(int), reinterpret_cast<const char*>(&CmdPayloadLen));
        SetBuffValue(&CmdBuff[CmdBuffPos], sizeof(char) * 32, reinterpret_cast<const char*>(&CmdSession));
        
        CmdPayloadLen   =0;
    }
    void SetCmdPayloadLen()
    {
        memcpy_s(&CmdBuff[sizeof(short)+sizeof(short)], sizeof(int), reinterpret_cast<const char*>(&CmdPayloadLen), sizeof(int));
    }
    
    



};




struct _LoginInfo
{
	char m_szUserName[MAX_BUFF_50];
	char m_szUserPass[MAX_BUFF_50];
	int  m_nSendCount;
	int  m_nServerSuccess;
	int  m_nServerFail;

	_LoginInfo()
	{
		m_szUserName[0]  = '\0';
		m_szUserPass[0]  = '\0';
		m_nSendCount     = 0;
		m_nServerSuccess = 0;
		m_nServerFail    = 0;
	}
};

struct _ServerInfo
{
	char m_szServerIP[MAX_BUFF_50];
	int  m_nServerPort;

	_ServerInfo()
	{
		m_szServerIP[0] = '\0';
		m_nServerPort   = 0;
	}
};

struct _LoginClient
{
	char m_szUserName[MAX_BUFF_50];
	char m_szUserPass[MAX_BUFF_50];
	int  m_nUserIDFrom;
	int  m_nUserIDTo;

	_LoginClient()
	{
		m_szUserName[0] = '\0';
		m_szUserPass[0] = '\0';
		m_nUserIDFrom   = 0;
		m_nUserIDTo     = 0;
	}
};

typedef vector<_LoginInfo> vecLoginInfo;


#define COMMAND_PLUGA    0x2201


struct _PassTest
{
        int m_nSendCount;   //压测发送数据包数
        int m_nRecvCount;   //压测接收数据包数

        _PassTest()
        {
                Init();
        }

        void Init()
        {
                m_nSendCount = 0;
                m_nRecvCount = 0;
        }
};


//注册有关命令结构信息,从字节流解码为实际命令,将命令编码为字节流用于发送

class CNetOperation
{
public:
    CNetOperation(int tcpudp);
    virtual ~CNetOperation();

public:
    void Init(int tcpudp);   //初始化网络环境
    void Deinit();              //销毁网络
    void Close();               //关闭网络
    bool Conn(const std::string& ParamServerIP, const int& ParamServerPort, const int& ParamTimeout);   //连接网络
    
    bool SendPacket(_LoginInfo& objLoginInfo,const char* ParamPacketBuff, const int& PacketLength); //发送报文
    bool RecvPacket(_LoginInfo& objLoginInfo,const char* ParamPacketBuff, const int& PacketLength); //接收报文

    bool GetRecvPktLength(const int& ParamPacketLength); //获得需要接收的下一个报文的长度
    void GetCmdRetInfo(const char* ParamPacketBuff, short& ParamCommandID, const int& ParamCmdRet); //获得接收到的命令ID以及执行返回结果

private:
    Cmd_Packet_Info Cmd_Packet;

public:
    bool Send_Login(_LoginInfo& objLoginInfo,const int &ParamCmdID,const std::string& ParamSvrUserName, const std::string& ParamSvrUserPass);
    bool Send_Logout(_LoginInfo& objLoginInfo,const int &ParamCmdID,const std::string& ParamSvrUserName);

public:
    bool Send_FileList(const std::string& ParamSvrUserName, const std::string& ParamRemotePath);
    bool Send_Download(const std::string& ParamSvrUserName, const std::string& ParamRemotePath, const std::string& ParamLocalPath,
        const std::string& ParamFileName, int nIndex,  int nSize, int& nBockCount);
        
    bool Send_Upload(const std::string& ParamSvrUserName,
        const std::string& ParamRemotePath,
        const std::string& ParamLocalPath,
        const std::string& ParamFileName);

public:
	bool Send_UserInfo(const int &ParamUserID ,int &ParamReUserID,int &ParamLife,int &ParamMagic);
	bool Send_SetUserInfo(int nUserID, int nLife, int nMagic);

public:
    bool Send_Plug(const char* ParamText ,short &ParamReCmdID,int &ParamRetB,int &ParamRetA ); 

private:
    bool  OpenLocalFile(const std::string& ParamLocalPath,  const std::string& ParamFileName);  //打开需要发送的本地文件
    void  closeLocalFile();
    long int GetFileSize();
    int  GetFileBlockCount(const int &ParamBufferSize);//计算需要发送的本地文件的数据块数量
    int  GetFileLastBlockSize(const int &ParamBufferSize);//计算需要发送的本地文件的数据块数量
    void  ReadFileBlock(const int& ParamBlockIndex, char* ParamFileBuffer);
    
    char FileBuffer[MAX_BUFF_10240] = {'\0'};
    std::ifstream localfile;
    int  LocalFileSize;
    int  FileBlockSize;
    int FileBlockCount;
    int LastBlockSize;
    
private:
    CSockWrap* m_sckClient;
    std::vector<FileInfo> FileList;
};

#endif // CNetOperation

