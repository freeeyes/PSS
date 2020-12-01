#ifndef _MESSAGEDEFINE_H
#define _MESSAGEDEFINE_H

//���ﴦ�����еĽ������������
//��Щ������Ծ�̬�����ķ�ʽ��¶����
//���������ڴ�����Ķ�����Ϣ��ӳ��
//��ô����ԭ���ǣ����������if else ��������
//�Ż�����ṹ������sonar��������
//add by freeeyes

#include <ace/OS_NS_sys_resource.h>

#include "define.h"
#include "ForbiddenIP.h"
#include <string>

//�������ֵ���Ͷ���
enum class EM_CONSOLE_MESSAGE
{
    CONSOLE_MESSAGE_SUCCESS = 0,
    CONSOLE_MESSAGE_FAIL    = 1,
    CONSOLE_MESSAGE_CLOSE   = 2,
};

static const string COMMAND_SPLIT_STRING = " ";

//��������
class _CommandInfo
{
public:
    uint8 m_u1OutputType                = 0;        //������ͣ�0Ϊ�����ƣ�1Ϊ�ı�
    string m_strCommandTitle;                       //��������ͷ
    string m_strCommandExp;                         //����������չ����
    string m_strUser  = {'\0'};                     //�û���Ϣ

    _CommandInfo() = default;
};

//�ļ����ṹ
class _FileInfo
{
public:
    string m_strFilePath;
    string m_strFileName;
    string m_strFileParam;

    _FileInfo() = default;
};

//�����˿���Ϣ
class _ListenInfo
{
public:
    uint16 m_u2Port                  = 0;
    uint32 m_u4PacketParseID         = 0;
    uint8  m_u1IPType                = TYPE_IPV4;
    string m_strListenIP;

    _ListenInfo() = default;
};

//ȾɫIP��Ϣ
class _DyeIPInfo
{
public:
    string m_strClientIP             = {'\0'};   //Ⱦɫ�ͻ���IP
    uint16 m_u2MaxCount              = 0;        //�������
};

//Ⱦɫ��CommandID
class _DyeCommandInfo
{
public:
    uint16 m_u2CommandID = 0;               //Ⱦɫ�ͻ�������
    uint16 m_u2MaxCount  = 0;                //�������
};

class _PoolName
{
public:
    string m_strPoolName             = {'\0'};   //�ڴ������
    bool   m_blState                 = false;    //��ǰ�ڴ�ش�����Ϣ״̬
};

//Э�����, �������ݲ���
bool GetCommandParam(const string& strCommand, const char* pTag, string& strValue);                             //������ָ����Command����
bool GetFileInfo(const string& strFile, _FileInfo& FileInfo);                                                 //��һ��ȫ·���зֳ��ļ���
bool GetForbiddenIP(const string& Command, _ForbiddenIP& ForbiddenIP);                                     //�õ���ֹ��IP�б�
bool GetConnectServerID(const string& strCommand, int& nServerID);                                            //�õ�һ��ָ���ķ�����ID
bool GetDebug(const string& strCommand, uint8& u1Debug);                                                      //�õ���ǰ���õ�BUDEG
bool GetTrackIP(const string& strCommand, _ForbiddenIP& ForbiddenIP);                                         //�õ����õ�׷��IP
bool GetLogLevel(const string& strCommand, int& nLogLevel);                                                   //�õ���־�ȼ�
bool GetAIInfo(const string& strCommand, int& nAI, int& nDispose, int& nCheck, int& nStop);                   //�õ�AI����
bool GetNickName(const string& strCommand, string& strName);                                                  //�õ����ӱ���
bool GetConnectID(const string& strCommand, uint32& u4ConnectID, bool& blFlag);                               //�õ�ConnectID
bool GetMaxConnectCount(const string& strCommand, uint16& u2MaxConnectCount);                                 //�õ�������������
bool GetListenInfo(const string& strCommand, _ListenInfo& objListenInfo);                                     //�õ������˿���Ϣ
bool GetTestFileName(const string& strCommand, string& strFileName);                                          //��ü��ز����ļ���
bool GetDyeingIP(const string& strCommand, _DyeIPInfo& objDyeIPInfo);                                         //���ȾɫIP�������Ϣ
bool GetDyeingCommand(const string& strCommand, _DyeCommandInfo& objDyeCommandInfo);                          //���ȾɫCommand�������Ϣ
bool GetPoolSet(const string& strCommand, _PoolName& objPoolName);                                            //����ڴ�ص�����

#endif
