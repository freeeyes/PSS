#ifndef _FILETESTMANAGER_H_
#define _FILETESTMANAGER_H_

#include "HashTable.h"
#include "XmlOpeation.h"
#include "HandlerManager.h"

#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"
#include "FileTest.h"
#include <sstream>
#include <unordered_map>

class CFileTestManager : public ACE_Task<ACE_MT_SYNCH>, public IFileTestManager
{
public:
    CFileTestManager(void) = default;

    //�ļ����Է���
    FileTestResultInfoSt FileTestStart(const char* szXmlFileTestName);      //��ʼ�ļ�����
    int FileTestEnd();                                                      //�����ļ�����
    void HandlerServerResponse(uint32 u4ConnectID) final;                   //��ǰ���ӷ������ݰ��Ļص�����
    void Close();                                                           //����

    int handle_timeout(const ACE_Time_Value& tv, const void* arg) final;

private:
    bool LoadXmlCfg(const char* szXmlFileTestName, FileTestResultInfoSt& objFileTestResult);        //��ȡ���������ļ�
    FILE_TEST_RESULT ReadTestFile(const char* pFileName, int nType, FileTestDataInfoSt& objFileTestDataInfo) const;   //����Ϣ���ļ��������ݽṹ
    int  InitResponseRecordList();                                                                  //��ʼ��ResponseRecord
    bool AddResponseRecordList(uint32 u4ConnectID, const ACE_Time_Value& tv);                       //���ResponseRecordList

    ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;
    //�ļ����Ա���
    bool m_bFileTesting       = false;       //�Ƿ����ڽ����ļ�����
    int32 m_n4TimerID         = 0;           //��ʱ��ID

    CXmlOpeation m_MainConfig;
    string m_strProFilePath;
    uint32 m_u4TimeInterval   = 1000;   //��ʱ���¼����
    uint32 m_u4ConnectCount   = 0;      //ģ��������
    uint32 m_u4ResponseCount  = 0;      //������Ӧ������
    uint32 m_u4ExpectTime     = 0;      //��������ʱ�ܵĺ�ʱ(��λ����)
    uint32 m_u4ParseID        = 0;      //������ID
    uint32 m_u4ContentType    = 0;      //Э����������,1�Ƕ�����Э��,0���ı�Э��

    using vecFileTestDataInfoSt = vector<FileTestDataInfoSt>;
    vecFileTestDataInfoSt m_vecFileTestDataInfoSt;

    //�ڲ�ʹ�õ�ӳ����
    class ResponseRecordSt
    {
    public:
        uint64 m_u8StartTime     = 0;
        uint8  m_u1ResponseCount = 0;
        uint32 m_u4ConnectID     = 0;

        ResponseRecordSt() = default;
    } ;

    //������ղ���
    using hashmapResponseRecord = unordered_map<uint32, shared_ptr<ResponseRecordSt>>;
    hashmapResponseRecord m_objResponseRecordList;

};

using App_FileTestManager = ACE_Singleton<CFileTestManager, ACE_Null_Mutex>;
#endif //_FILETESTMANAGER_H_