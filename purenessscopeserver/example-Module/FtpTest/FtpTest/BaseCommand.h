#pragma once

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"
#include "DirView.h"

#include <string>
#include <map>

//定义客户端信令(TCP)
#define COMMAND_LOGIN                  0x2000     //登陆
#define COMMAND_LOGOUT                 0x2001     //退出 
#define COMMAND_FILELIST               0x2002     //文件，文件夹列表
#define COMMAND_FILE_DOWNLOAD          0x2003     //文件下载
#define COMMAND_FILE_UPLOAD            0x2004     //文件上传
#define COMMAND_RETURN_LOGIN           0xe000     //登陆应答 
#define COMMAND_RETURN_LOGOUT          0xe001     //登出应答
#define COMMAND_RETURN_FILELIST        0xe002     //文件，文件夹列表应答
#define COMMAND_RETURN_FILE_DOWNLOAD   0xe003     //文件下载
#define COMMAND_RETURN_FILE_UPLOAD     0xe004     //文件上传

//定义常用变量
#define LOGIN_SUCCESS            0
#define LOGIN_FAIL_NOEXIST       1
#define LOGIN_FAIL_ONLINE        2
#define LOGIN_FAIL_PASSWORD      3

#define OP_OK                    0
#define OP_FAIL                  1

using namespace std;

//用户登陆情况结构体
struct _UserInfo
{
    char   m_szUserPass[MAX_BUFF_100];   //用户密码
    bool   m_blOnline;                   //是否在线 false为不在线，true为在线
    uint32 m_u4ConnectID;                //ConnectID

    _UserInfo()
    {
        m_szUserPass[0] = '\0';
        m_blOnline      = false;
        m_u4ConnectID   = 0;
    }
};


typedef map<string, _UserInfo*> mapUserList; //用户列表

class CBaseCommand : public CClientCommand
{
public:
    CBaseCommand(void);
    ~CBaseCommand(void);

    void InitUserList();
    void ClearUserList();
    int DoMessage(IMessage* pMessage, bool& bDeleteFlag);
    void SetServerObject(CServerObject* pServerObject);

private:
    int  CheckFtpUser(const char* pUserName, const char* pUserPass, uint32 u4ConnectID);  //检测账号密码
    void LeaveFtpUser(uint32 u4ConnectID);        //连接断开的时候退出登录状态
    void LeaveFtpUser(const char* pUserName);     //用户登出的时候退出登录状态
    bool CheckOnlineUser(const char* pUserName, uint32 u4ConnectID);  //比较在线用户是否有操作权限

    void Do_Ftp_Login(IMessage* pMessage);        //处理登陆
    void Do_Ftp_Logout(IMessage* pMessage);       //处理登出
    void Do_Ftp_FileList(IMessage* pMessage);     //获得文件列表
    void Do_Ftp_FileDownLoad(IMessage* pMessage); //文件下载
    void Do_Ftp_FileUpLoad(IMessage* pMessage);   //文件上传

private:
    CServerObject* m_pServerObject;
    mapUserList    m_mapUserList;                   //用户列表
};

