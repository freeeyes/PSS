#ifndef _LISTMANAGER_H
#define _LISTMANAGER_H

//����������б�
//add by freeeyes

#include "define.h"
#include "IBuffPacket.h"
#include "iniparser.h"

#define LS_LIST_FILENAME "ListServer.ini"  //���ػ��洢���ļ���
#define LG_LIST_MD5      "NOFILE"          //����Ĭ��MD5 

//LG������������Ϣ
struct _LG_Info
{
    uint32      m_u4ConnectID;
    uint32      m_u4LGID;
    char        m_szIP[50];
    uint32      m_u4Port;

    _LG_Info()
    {
        m_u4ConnectID = 0;
        m_u4LGID      = 0;
        m_szIP[0]     = '\0';
        m_u4Port      = 0;
    }
};

class ClistManager
{
public:
    ClistManager();
    ~ClistManager();

    void Clear();

    void Add_LG_Info(uint32 u4ConnectID, uint32 u4LGID, const char* pIP, uint32 u4Port);
    _LG_Info* Get_LG_Info_By_LGID(uint32 u4LGID);
    _LG_Info* Get_LG_Info_By_ConnectID(uint32 u4ConnectID);
    _LG_Info* Get_LG_Info_By_Index(uint32 u4Index);
    void Del_LG_Info_By_ConnectID(uint32 u4ConnectID);
    uint32 Get_LG_Count();

    //�õ������������б���Ϣ
    void Get_All_LG_List(IBuffPacket* pbpList, uint32& u4Count);

    //����MD5ֵ
    void Set_MD5_Data(const char* pMD5);

    //�õ�ָ�����б�汾��Ϣ
    char* Get_MD5_Data();

    void SaveList();
    void ReadList();

private:
    typedef vector<_LG_Info>   vecLGInfo;
    vecLGInfo                  m_vecLGInfo;
    char                       m_szMD5[33];
    ACE_Recursive_Thread_Mutex m_ThreadLock;
};

#endif
