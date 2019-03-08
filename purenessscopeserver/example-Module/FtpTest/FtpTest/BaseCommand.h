#pragma once

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"
#include "DirView.h"

#include <string>
#include <map>

//����ͻ�������(TCP)
#define COMMAND_LOGIN                  0x2000     //��½
#define COMMAND_LOGOUT                 0x2001     //�˳� 
#define COMMAND_FILELIST               0x2002     //�ļ����ļ����б�
#define COMMAND_FILE_DOWNLOAD          0x2003     //�ļ�����
#define COMMAND_FILE_UPLOAD            0x2004     //�ļ��ϴ�
#define COMMAND_RETURN_LOGIN           0xe000     //��½Ӧ�� 
#define COMMAND_RETURN_LOGOUT          0xe001     //�ǳ�Ӧ��
#define COMMAND_RETURN_FILELIST        0xe002     //�ļ����ļ����б�Ӧ��
#define COMMAND_RETURN_FILE_DOWNLOAD   0xe003     //�ļ�����
#define COMMAND_RETURN_FILE_UPLOAD     0xe004     //�ļ��ϴ�

//���峣�ñ���
#define LOGIN_SUCCESS            0
#define LOGIN_FAIL_NOEXIST       1
#define LOGIN_FAIL_ONLINE        2
#define LOGIN_FAIL_PASSWORD      3

#define OP_OK                    0
#define OP_FAIL                  1

using namespace std;

//�û���½����ṹ��
struct _UserInfo
{
    char   m_szUserPass[MAX_BUFF_100];   //�û�����
    bool   m_blOnline;                   //�Ƿ����� falseΪ�����ߣ�trueΪ����
    uint32 m_u4ConnectID;                //ConnectID

    _UserInfo()
    {
        m_szUserPass[0] = '\0';
        m_blOnline      = false;
        m_u4ConnectID   = 0;
    }
};


typedef map<string, _UserInfo*> mapUserList; //�û��б�

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
    int  CheckFtpUser(const char* pUserName, const char* pUserPass, uint32 u4ConnectID);  //����˺�����
    void LeaveFtpUser(uint32 u4ConnectID);        //���ӶϿ���ʱ���˳���¼״̬
    void LeaveFtpUser(const char* pUserName);     //�û��ǳ���ʱ���˳���¼״̬
    bool CheckOnlineUser(const char* pUserName, uint32 u4ConnectID);  //�Ƚ������û��Ƿ��в���Ȩ��

    void Do_Ftp_Login(IMessage* pMessage);        //�����½
    void Do_Ftp_Logout(IMessage* pMessage);       //����ǳ�
    void Do_Ftp_FileList(IMessage* pMessage);     //����ļ��б�
    void Do_Ftp_FileDownLoad(IMessage* pMessage); //�ļ�����
    void Do_Ftp_FileUpLoad(IMessage* pMessage);   //�ļ��ϴ�

private:
    CServerObject* m_pServerObject;
    mapUserList    m_mapUserList;                   //�û��б�
};

