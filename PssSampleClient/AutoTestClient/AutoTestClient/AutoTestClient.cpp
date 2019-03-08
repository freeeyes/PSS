// AutoTestClient.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "XmlOpeation.h"
#include "sock_wrap.h"

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#define XML_PATH "XML_Packet"
#define XML_FILE_RESULT "Auto_Test_Resault.html"

#ifdef WIN32
typedef HANDLE Thread_Mutex;
#else
typedef pthread_mutex_t Thread_Mutex;
#endif

struct _Thread_Info
{
	FILE* pFile;
	_Command_Info* p_Command_Info;
	char szIP[MAX_BUFF_50];
	int nPort;
	short sOrder;
	int nThreadID;
	Thread_Mutex mutex;

	_Thread_Info()
	{
		pFile          = NULL;
		p_Command_Info = NULL;
		memset(szIP, 0, MAX_BUFF_50);
		nPort          = 0;
		sOrder         = 0;
		nThreadID      = 0;
	}
};

//�����߳���
Thread_Mutex Create_Mutex()
{
	Thread_Mutex mutex;
#ifdef WIN32
		mutex = CreateMutex(NULL, FALSE, NULL);
#else
		pthread_mutex_init(&mutex, NULL);
#endif
		return mutex;
}

//�����߳���
static void Close_Mutex(Thread_Mutex& mutex)
{
#ifdef WIN32
	CloseHandle(mutex);
#else
	pthread_mutex_destroy(&mutex);
#endif
}

//�߳�����
static void Lock(Thread_Mutex& mutex)
{
	//if(NULL != mutex)
	{
#ifdef WIN32
		WaitForSingleObject(mutex, INFINITE);
#else
		pthread_mutex_lock(&mutex);
#endif
	}
}

//�߳�������
static void UnLock(Thread_Mutex& mutex)
{
	//if(NULL != mutex)
	{
#ifdef WIN32
		ReleaseMutex(mutex);
#else
		pthread_mutex_unlock(&mutex);
#endif
	}
}

//���в����Ӽ�
#ifdef WIN32
DWORD WINAPI Run_Test(LPVOID arg)
#else
void* Run_Test(void * arg)
#endif
{
	//����Զ�̲���
	//ODSocket obj_ODSocket;
	string strContent;
	string strTime;

	_Thread_Info* p_Thread_Info = (_Thread_Info* )arg;

	printf("[Run_Test]obj_Command_Info.m_szCommandName=%s.\n", p_Thread_Info->p_Command_Info->m_szCommandName);

	//obj_ODSocket.Init();
	//obj_ODSocket.Create(AF_INET, SOCK_STREAM, 0);
	//bool blState = obj_ODSocket.Connect(p_Thread_Info->szIP, p_Thread_Info->nPort, 5);

	InitializeSocketEnvironment();
	HSocket obj_ODSocket    =SocketOpen(SOCK_STREAM);
	sockaddr_in addr;
	GetAddressFrom(&addr, p_Thread_Info->szIP, p_Thread_Info->nPort);
	bool blState = SocketConnect(obj_ODSocket, &addr);
	if(false == blState)
	{
		Lock(p_Thread_Info->mutex);
		Create_TD_Content(p_Thread_Info->pFile, "error", p_Thread_Info->p_Command_Info->m_szCommandName, "���ӽ���ʧ��", strTime.c_str());
		UnLock(p_Thread_Info->mutex);
		return 0;
	}

	bool blIsError = false;
	unsigned long llBegin = GetSystemTickCount();
	for(int i = 0; i < p_Thread_Info->p_Command_Info->m_nCount; i++)
	{
		//��ʼ��������
		int nSendLen = p_Thread_Info->p_Command_Info->m_obj_Packet_Send.Get_Length();
		char* pSend = new char[nSendLen];
		p_Thread_Info->p_Command_Info->m_obj_Packet_Send.In_Stream(pSend, nSendLen, p_Thread_Info->sOrder);

		bool blSendFlag = false;
		int nCurrSend = 0;
		while(true)
		{
			//int nDataLen = obj_ODSocket.Send(&pSend[nCurrSend], nSendLen - nCurrSend);

			transresult_t rt;
			SocketSend(obj_ODSocket, &pSend[nCurrSend], nSendLen - nCurrSend, rt);
			int nDataLen = rt.nbytes;
			if(nDataLen < 0)
			{
				strContent = "�������ݰ�ʧ��";
				blIsError = true;
				break;
			}
			else if(nDataLen == nSendLen - nCurrSend)
			{
				blSendFlag = true;
				break;
			}
			else
			{
				nCurrSend += nDataLen;
			}
		}
		delete pSend;

		//�ж��Ƿ��������
		if(blIsError == true)
		{
			break;
		}

		int nRecvLen = p_Thread_Info->p_Command_Info->m_obj_Packet_Recv.Get_Length();
		if(nRecvLen > 0 && blSendFlag == true)
		{
			//��Ҫ���ܷ�����֤���ݰ�
			char* pRecv = new char[nRecvLen];

			int nCurrRecv = 0;
			while(true)
			{
				//int nDataLen = obj_ODSocket.Recv(&pRecv[nCurrRecv], nRecvLen - nCurrRecv);

                transresult_t rt;
                SocketRecv(obj_ODSocket, &pRecv[nCurrRecv], nRecvLen - nCurrRecv, rt);
                int nDataLen = rt.nbytes;

				if(nDataLen <= 0)
				{
					strContent = "���շ������ݰ�ʧ��";
					blIsError = true;
					break;
				}
				else if(nDataLen == nRecvLen - nCurrRecv)
				{
					//����������ݰ�
					strContent = p_Thread_Info->p_Command_Info->m_obj_Packet_Recv.Check_Stream(pRecv, nRecvLen, p_Thread_Info->sOrder, blIsError);
					break;
				}
				else
				{
					//�����հ�
					nCurrRecv += nDataLen;
				}
			}
			delete pRecv;
		}

		//�ж��Ƿ��������
		if(blIsError == true)
		{
			break;
		}
	}

	//����ʱ������
	unsigned long llEnd = GetSystemTickCount();
	int nCostTime = (int)(llEnd - llBegin);

	char szTimeContent[MAX_BUFF_500] = {'\0'};
	if(nCostTime <= p_Thread_Info->p_Command_Info->m_nTimeCost)
	{
		sprintf_safe(szTimeContent, MAX_BUFF_500, "Ҫ��ʱ��[%d]���� ʵ������ʱ��[%d]���룬����", p_Thread_Info->p_Command_Info->m_nTimeCost, nCostTime);
	}
	else
	{
		sprintf_safe(szTimeContent, MAX_BUFF_500, "Ҫ��ʱ��[%d]���� ʵ������ʱ��[%d]���룬�쳣", p_Thread_Info->p_Command_Info->m_nTimeCost, nCostTime);
	}
	strTime = szTimeContent;

	if(blIsError == true)
	{
		Lock(p_Thread_Info->mutex);
		if(p_Thread_Info->nThreadID == 0)
		{
			Create_TD_Content(p_Thread_Info->pFile, "error", p_Thread_Info->p_Command_Info->m_szCommandName, strContent.c_str(), strTime.c_str());
		}
		else
		{
			char szName[MAX_BUFF_100] = {'\0'};
			sprintf_safe(szName, MAX_BUFF_100, "%s(�߳�ID:%d)",  p_Thread_Info->p_Command_Info->m_szCommandName, p_Thread_Info->nThreadID);
			Create_TD_Content(p_Thread_Info->pFile, "error", szName, strContent.c_str(), strTime.c_str());
		}
		UnLock(p_Thread_Info->mutex);
	}
	else
	{
		Lock(p_Thread_Info->mutex);
		if(p_Thread_Info->nThreadID == 0)
		{
			Create_TD_Content(p_Thread_Info->pFile, "content", p_Thread_Info->p_Command_Info->m_szCommandName, strContent.c_str(), strTime.c_str());
		}
		else
		{
			char szName[MAX_BUFF_100] = {'\0'};
			sprintf_safe(szName, MAX_BUFF_100, "%s(�߳�ID:%d)",  p_Thread_Info->p_Command_Info->m_szCommandName, p_Thread_Info->nThreadID);
			Create_TD_Content(p_Thread_Info->pFile, "content", szName, strContent.c_str(), strTime.c_str());
		}
		UnLock(p_Thread_Info->mutex);
	}
	//obj_ODSocket.Close();
	SocketClose(obj_ODSocket);

	FreeSocketEnvironment();

	return 0;
}

//���в��Լ�����ý��
void Run_Assemble_List(vec_Test_Assemble obj_Test_Assemble_List)
{
	FILE* pFile = fopen(XML_FILE_RESULT, "w");
	if(NULL == pFile)
	{
		printf("[Run_Assemble_List](%s) File Create fail.\n");
		return;
	}

	//����html�ļ�ͷ
	Create_HTML_Begin(pFile);

	for(int i = 0; i < (int)obj_Test_Assemble_List.size(); i++)
	{
		Create_TD_Title(pFile, obj_Test_Assemble_List[i].m_szTestAssembleName, obj_Test_Assemble_List[i].m_szDesc,
						obj_Test_Assemble_List[i].m_szIP, obj_Test_Assemble_List[i].m_nPort);

		short sOrder = 0;
		if(strcmp(obj_Test_Assemble_List[i].m_szOrder, "NET") == 0)
		{
			sOrder = 1;
		}

		for(int j = 0; j < (int)obj_Test_Assemble_List[i].m_obj_Command_Info_List.size(); j++)
		{
			Thread_Mutex objmutex = Create_Mutex();

			//�鿴�Ƿ�����Ҫ���߳�
			if(obj_Test_Assemble_List[i].m_obj_Command_Info_List[j].m_nThreadCount == 1)
			{
				_Thread_Info* p_Thread_Info = new _Thread_Info();
				p_Thread_Info->pFile          = pFile;
				p_Thread_Info->p_Command_Info = &obj_Test_Assemble_List[i].m_obj_Command_Info_List[j];
				sprintf_safe(p_Thread_Info->szIP, 50, "%s", obj_Test_Assemble_List[i].m_szIP);
				p_Thread_Info->nPort          = obj_Test_Assemble_List[i].m_nPort;
				p_Thread_Info->sOrder         = sOrder;
				p_Thread_Info->mutex          = objmutex;

				Run_Test((void* )p_Thread_Info);

				delete p_Thread_Info;
			}
			else
			{
				//�������̲߳���
#ifdef WIN32
				_Thread_Info* threadinfoarray = new _Thread_Info[obj_Test_Assemble_List[i].m_obj_Command_Info_List[j].m_nThreadCount];
				HANDLE* handleArray = new HANDLE[obj_Test_Assemble_List[i].m_obj_Command_Info_List[j].m_nThreadCount];
				for(int k = 0; k < obj_Test_Assemble_List[i].m_obj_Command_Info_List[j].m_nThreadCount; k++)
				{
					threadinfoarray[k].pFile          = pFile;
					threadinfoarray[k].p_Command_Info = &obj_Test_Assemble_List[i].m_obj_Command_Info_List[j];
					sprintf_safe(threadinfoarray[k].szIP, MAX_BUFF_50, "%s", obj_Test_Assemble_List[i].m_szIP);
					threadinfoarray[k].nPort          = obj_Test_Assemble_List[i].m_nPort;
					threadinfoarray[k].sOrder         = sOrder;
					threadinfoarray[k].mutex          = objmutex;
					threadinfoarray[k].nThreadID      = k + 1;

					DWORD ThID;
					handleArray[k] = CreateThread(NULL, 0, Run_Test, (void* )&threadinfoarray[k], 0, &ThID);
				}

				//�ȴ������߳̽���
				WaitForMultipleObjects(2, handleArray, TRUE, INFINITE);
				delete[] handleArray;
				delete[] threadinfoarray;
#else
				_Thread_Info* threadinfoarray = new _Thread_Info[obj_Test_Assemble_List[i].m_obj_Command_Info_List[j].m_nThreadCount];
				pthread_t* handleArray = new pthread_t[obj_Test_Assemble_List[i].m_obj_Command_Info_List[j].m_nThreadCount];
				for(int k = 1; k <= obj_Test_Assemble_List[i].m_obj_Command_Info_List[j].m_nThreadCount; k++)
				{
					threadinfoarray[k].pFile          = pFile;
					threadinfoarray[k].p_Command_Info = &obj_Test_Assemble_List[i].m_obj_Command_Info_List[j];
					sprintf_safe(threadinfoarray[k].szIP, MAX_BUFF_50, "%s", obj_Test_Assemble_List[i].m_szIP);
					threadinfoarray[k].nPort          = obj_Test_Assemble_List[i].m_nPort;
					threadinfoarray[k].sOrder         = sOrder;
					threadinfoarray[k].mutex          = objmutex;
					threadinfoarray[k].nThreadID      = k;

					pthread_create(&handleArray[k], NULL, &Run_Test, (void* )&threadinfoarray[k]);
				}

				//�ȴ������߳̽���
				for(int k = 1; k <= obj_Test_Assemble_List[i].m_obj_Command_Info_List[j].m_nThreadCount; k++)
				{
					pthread_join(handleArray[k], NULL);
				}

				delete[] handleArray;
				delete[] threadinfoarray;
#endif

			}
			Close_Mutex(objmutex);
		}
	}
	Create_HTML_End(pFile);
}


int main(int argc, char* argv[])
{
	vec_Xml_File_Name obj_vec_Xml_File_Name;
	CXmlOpeation      obj_XmlOpeation;
	vec_Test_Assemble obj_Test_Assemble_List;

	bool blRet = Read_Xml_Folder(XML_PATH, obj_vec_Xml_File_Name);
	if(false == blRet)
	{
		printf("[Main]Get XML path error.\n");
		printf("[Main]please any key to exit.\n");
		getchar();
	}

	for(int i = 0; i < (int)obj_vec_Xml_File_Name.size(); i++)
	{
		_Test_Assemble obj_Test_Assemble;
		obj_XmlOpeation.Parse_XML_Test_Assemble(obj_vec_Xml_File_Name[i].c_str(), obj_Test_Assemble);
		obj_Test_Assemble_List.push_back(obj_Test_Assemble);
	}

	//���в�������
	Run_Assemble_List(obj_Test_Assemble_List);

	return 0;
}

