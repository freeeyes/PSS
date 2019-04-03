/***************************************************************
 * Name:      CNetOperation.cpp
 * Purpose:   Code for Application Frame
 * Author:    smith ()
 * Created:   2019-02-21
 * Copyright: smith ()
 * License:
 **************************************************************/

#include "CNetOperation.h"

#define _LOG2FILE_
#include "Mylog.h"



CNetOperation::CNetOperation(int tcpudp )
{
    this->Init(tcpudp);
}

CNetOperation::~CNetOperation( )
{
    this->Deinit();
}
void CNetOperation::Init(int tcpudp)
{
    //CLog2File::Initialize();
    m_log.InitLogFile("Mylog.log");

    //初始化TCP链接
    InitializeSocketEnvironment();
    this->m_sckClient =new CSockWrap(tcpudp);
}

void CNetOperation::Deinit()
{
    //初始化TCP链接
    FreeSocketEnvironment();
    this->Close();
}

void CNetOperation::Close()
{
    if(IsValidSocketHandle(m_sckClient->GetHandle()) )
    {
            this->m_sckClient->Close();
    }
}

bool CNetOperation::Conn(const std::string  &ParamServerIP,const int  &ParamServerPort,const int &ParamTimeout)
{
	//socket创建的准备工作

    m_sckClient->SetAddress(ParamServerIP.c_str(), ParamServerPort);
    m_sckClient->SetTimeOut(ParamTimeout,-1,-1);

    m_sckClient->Reopen(true);
    return  (GetLastSocketError() !=0);
    
}



bool CNetOperation::SendPacket(_LoginInfo& objLoginInfo,const char* ParamPacketBuff,const int &PacketLength)          //发送报文
{    
	int nTotalSendLen = PacketLength;
	int nBeginSend    = 0;
	int nCurrSendLen  = 0;

	while(true)
	{
        transresult_t   rt  =this->m_sckClient->Send((void*)(ParamPacketBuff + nBeginSend), nTotalSendLen);
        nCurrSendLen    =rt.nbytes;
		if(nCurrSendLen <= 0)
		{
            int dwError = GetLastSocketError();
            logError("向远程服务器发送数据失败!!!");
            objLoginInfo.m_nServerFail++;
			return false;
		}
		else
		{
			nTotalSendLen -= nCurrSendLen;
			if(nTotalSendLen == 0)
			{
				//发送完成
                logDebug("向远程服务器发送数据完成!!!");
				break;
			}
			else
			{
				nBeginSend += nCurrSendLen;
			}
		}
    }	
    objLoginInfo.m_nSendCount++;
    return  true;

}

bool CNetOperation::RecvPacket(_LoginInfo& objLoginInfo,const char* ParamPacketBuff,const int &PacketLength)   //接收报文
{
	int nRecvLength = PacketLength;
	int nRecvBegin  = 0;
    int nCurrRecvLen  = 0;
    
	while(true)
	{
		if(nRecvLength - nRecvBegin == 0)
		{
			break;
		}
        
        transresult_t  rt  =m_sckClient->Recv((char*)(ParamPacketBuff + nRecvBegin), nRecvLength - nRecvBegin);
        nCurrRecvLen    =rt.nbytes;

		if(nCurrRecvLen <= 0)
		{
            int dwError = GetLastSocketError();
            logError("接收远程服务器数据失败!!!");
            objLoginInfo.m_nServerFail++;
			return false;
		}
		else
		{
			nRecvBegin += nCurrRecvLen;
		}
	}
    return  true;    
}    
    
    
bool CNetOperation::GetRecvPktLength(const int &ParamPacketLength)   //获得需要接收的下一个报文的长度
{
    _LoginInfo objLoginInfo;
    bool    Ret =this->RecvPacket(objLoginInfo,reinterpret_cast <const char*>(&ParamPacketLength),sizeof(const int));
    return  Ret;    
}



void CNetOperation::GetCmdRetInfo(const char* ParamPacketBuff,short &ParamCommandID,const int &ParamCmdRet)   //获得接收到的命令ID以及执行返回结果
{    
    memcpy_s((void*)(&ParamCommandID),  sizeof(short), reinterpret_cast <const char*>(&ParamPacketBuff), sizeof(short));
    memcpy_s((void*)(&ParamCmdRet),  sizeof(int), reinterpret_cast <const char*>(&ParamPacketBuff[sizeof(short)]), sizeof(int));
}   
    


bool CNetOperation::Send_Login(_LoginInfo& objLoginInfo,const int &ParamCmdID,const std::string  &ParamSvrUserName,const std::string  &ParamSvrUserPass)
{    

    Cmd_Packet.CmdVersion   =1;
    Cmd_Packet.CmdBuffPos   =0;
    Cmd_Packet.CmdID        = ParamCmdID;    
    Cmd_Packet.CmdBuffPos   =0;
	sprintf_s(Cmd_Packet.CmdSession, 32, "FREEEYES");

    Cmd_Packet.SetCmdHead();
    Cmd_Packet.SetCmdPayload( ParamSvrUserName );
    Cmd_Packet.SetCmdPayload( ParamSvrUserPass );
    Cmd_Packet.SetCmdPayloadLen();
    
    if(IsValidSocketHandle(m_sckClient->GetHandle()) )
    {
        return false;
    };
    
    const   char * CmdPacket    =Cmd_Packet.CmdBuff;      
    this->SendPacket(objLoginInfo,CmdPacket,Cmd_Packet.CmdBuffPos);   
            
    
    int RetPacketLength =0;
    this->GetRecvPktLength(RetPacketLength);

    //Cmd_Packet.CmdBuffPos   =0;
    char *Cmd_RevBuff	=new char[RetPacketLength];

    this->RecvPacket(objLoginInfo,Cmd_RevBuff,RetPacketLength);   //接收报文
    
	short RecvCommandID = 0;
	int CmdRet           = 0;    
    this->GetCmdRetInfo(Cmd_RevBuff, RecvCommandID,CmdRet);   //获得接收到的命令ID以及执行返回结果
	if (Cmd_RevBuff)  delete Cmd_RevBuff;

	if(CmdRet == LOGIN_FAIL_NOEXIST)
	{
        logError("用户名不存在!!!");
		return false;
	}
	else if(CmdRet == LOGIN_FAIL_ONLINE)
	{
        logError("此用户正在线，不能登录!!!");
		return false;
	}
	else if(CmdRet == LOGIN_FAIL_PASSWORD)
	{
        logError("密码错误!!!");
		return false;
	}


	if(CmdRet == LOGIN_SUCCESS)
	{
		objLoginInfo.m_nServerSuccess++;
	}
	else
	{
		objLoginInfo.m_nServerFail++;
	}


	return true;
}

bool CNetOperation::Send_Logout(_LoginInfo& objLoginInfo,const int &ParamCmdID,const std::string  &ParamSvrUserName)
{  
    Cmd_Packet.CmdVersion   =1;
    Cmd_Packet.CmdBuffPos   =0;
    Cmd_Packet.CmdID        = ParamCmdID;    
    Cmd_Packet.CmdBuffPos   =0;
	sprintf_s(Cmd_Packet.CmdSession, 32, "FREEEYES");

    Cmd_Packet.SetCmdHead();
    Cmd_Packet.SetCmdPayload( ParamSvrUserName );
    Cmd_Packet.SetCmdPayloadLen();
    
    if(IsValidSocketHandle(m_sckClient->GetHandle()) )
    {
        return false;
    };
    
    const   char * CmdPacket    =Cmd_Packet.CmdBuff;   
    this->SendPacket(objLoginInfo,CmdPacket,Cmd_Packet.CmdBuffPos);
        
        
    int RetPacketLength =0;
    this->GetRecvPktLength(RetPacketLength);

    //Cmd_Packet.CmdBuffPos   =0;

	char *Cmd_RevBuff = new char[RetPacketLength];

    this->RecvPacket(objLoginInfo,Cmd_RevBuff,RetPacketLength);   //接收报文
    
	short RecvCommandID = 0;
	int CmdRet           = 0;    
    this->GetCmdRetInfo(Cmd_RevBuff, RecvCommandID,CmdRet);   //获得接收到的命令ID以及执行返回结果
    
	if (Cmd_RevBuff)  delete Cmd_RevBuff;
    
	return true;
}


auto GetBuffValue(const char* ParamDesc, const int ParamSize, const char* ParamSrc,int ParamBufPos)
{
    memcpy_s((void*)ParamDesc, ParamSize, ParamSrc, ParamSize);
    ParamBufPos += ParamSize;    
}
    
    
bool CNetOperation::Send_FileList(const std::string  &ParamSvrUserName,const std::string  &ParamRemotePath)
{
        
    Cmd_Packet.CmdVersion   =1;
    Cmd_Packet.CmdBuffPos   =0;
    Cmd_Packet.CmdID        = COMMAND_FTP_FILELIST;    
    Cmd_Packet.CmdBuffPos   =0;
	sprintf_s(Cmd_Packet.CmdSession, 32, "FREEEYES");

    Cmd_Packet.SetCmdHead();
    Cmd_Packet.SetCmdPayload( ParamSvrUserName );
    Cmd_Packet.SetCmdPayload<short>( ParamRemotePath );
    
    Cmd_Packet.SetCmdPayloadLen();
    
    if(IsValidSocketHandle(m_sckClient->GetHandle()) )
    {
        return false;
    };
    
    _LoginInfo objLoginInfo;
    const   char * CmdPacket    =Cmd_Packet.CmdBuff;   
    this->SendPacket(objLoginInfo,CmdPacket,Cmd_Packet.CmdBuffPos);
             
    
    
    int RetPacketLength =0;
    this->GetRecvPktLength(RetPacketLength);

    //Cmd_Packet.CmdBuffPos   =0;


	char *Cmd_RevBuff = new char[RetPacketLength];

    int  Cmd_RevBuff_Pos    =0;   
    this->RecvPacket(objLoginInfo,Cmd_RevBuff,RetPacketLength);   //接收报文
    
	short RecvCommandID = 0;
	int CmdRet           = 0;    
    this->GetCmdRetInfo(Cmd_RevBuff, RecvCommandID,CmdRet);   //获得接收到的命令ID以及执行返回结果
    Cmd_RevBuff_Pos =sizeof(short)+sizeof(int);
        
   

	if(CmdRet == OP_OK)
	{
        int FileCount   =0;
        GetBuffValue(reinterpret_cast <const char*>(&FileCount),  sizeof(int), reinterpret_cast <const char*>(&Cmd_RevBuff[Cmd_RevBuff_Pos]),Cmd_RevBuff_Pos);

        this->FileList.clear();

		for(int i = 0; i < FileCount; i++)
		{
			int nFileNameLen = 0;
			char szFileName[MAX_BUFF_500] = {'\0'};
			int nFileType = IS_FILE;
			int nFileSize = 0;

            GetBuffValue(reinterpret_cast <const char*>(&nFileNameLen),  sizeof(char), reinterpret_cast <const char*>(&Cmd_RevBuff[Cmd_RevBuff_Pos]), Cmd_RevBuff_Pos);
            GetBuffValue(reinterpret_cast <const char*>(&szFileName),  nFileNameLen, reinterpret_cast <const char*>(&Cmd_RevBuff[Cmd_RevBuff_Pos]), Cmd_RevBuff_Pos);
            GetBuffValue(reinterpret_cast <const char*>(&nFileType),  sizeof(int), reinterpret_cast <const char*>(&Cmd_RevBuff[Cmd_RevBuff_Pos]), Cmd_RevBuff_Pos);
            GetBuffValue(reinterpret_cast <const char*>(&nFileSize),  sizeof(int), reinterpret_cast <const char*>(&Cmd_RevBuff[Cmd_RevBuff_Pos]), Cmd_RevBuff_Pos);


            this->FileList.push_back({szFileName,nFileType,nFileSize});

		}
	}
 	if (Cmd_RevBuff)  delete Cmd_RevBuff;
    
	return true;
}

bool CNetOperation::Send_Download(const std::string  &ParamSvrUserName,const std::string  &ParamRemotePath ,const std::string  &ParamLocalPath, const std::string  &ParamFileName, int nIndex, int nSize, int& nBockCount)
{  
       
    Cmd_Packet.CmdVersion   =1;
    Cmd_Packet.CmdBuffPos   =0;
    Cmd_Packet.CmdID        = COMMAND_FTP_FILE_DOWNLOAD;    
    Cmd_Packet.CmdBuffPos   =0;
	sprintf_s(Cmd_Packet.CmdSession, 32, "FREEEYES");

  
    Cmd_Packet.SetCmdHead();
    Cmd_Packet.SetCmdPayload( ParamSvrUserName );
    Cmd_Packet.SetCmdPayload<short>( ParamRemotePath );
    Cmd_Packet.SetCmdPayload<int>( nSize );
    Cmd_Packet.SetCmdPayload<int>( nIndex );       
    Cmd_Packet.SetCmdPayloadLen();
    
    if(IsValidSocketHandle(m_sckClient->GetHandle()) )
    {
        return false;
    };
    
    _LoginInfo objLoginInfo;
    const   char * CmdPacket    =Cmd_Packet.CmdBuff;   
    this->SendPacket(objLoginInfo,CmdPacket,Cmd_Packet.CmdBuffPos);
       
    
    
    int RetPacketLength =0;
    this->GetRecvPktLength(RetPacketLength);


	char *Cmd_RevBuff = new char[RetPacketLength];

    int  Cmd_RevBuff_Pos    =0;   
    this->RecvPacket(objLoginInfo,Cmd_RevBuff,RetPacketLength);   //接收报文
    
	short RecvCommandID = 0;
	int CmdRet           = 0;    
    this->GetCmdRetInfo(Cmd_RevBuff, RecvCommandID,CmdRet);   //获得接收到的命令ID以及执行返回结果
    Cmd_RevBuff_Pos =sizeof(short)+sizeof(int);
      
      
	if(CmdRet == OP_OK)
	{
		//得到文件块，并写入文件
		char* pBuffer = new char[nSize];
		int nFileBlockCount     = 0;
		int nFileCurrIndex      = 0;
		int nFileCurrBufferSize = 0;

        GetBuffValue(reinterpret_cast <const char*>(&nBockCount),  sizeof(int), reinterpret_cast <const char*>(&Cmd_RevBuff[Cmd_RevBuff_Pos]), Cmd_RevBuff_Pos);
        GetBuffValue(reinterpret_cast <const char*>(&nFileCurrIndex),  sizeof(int), reinterpret_cast <const char*>(&Cmd_RevBuff[Cmd_RevBuff_Pos]), Cmd_RevBuff_Pos);
        GetBuffValue(reinterpret_cast <const char*>(&nFileCurrBufferSize),  sizeof(int), reinterpret_cast <const char*>(&Cmd_RevBuff[Cmd_RevBuff_Pos]), Cmd_RevBuff_Pos);
        GetBuffValue(reinterpret_cast <const char*>(&pBuffer),  nFileCurrBufferSize, reinterpret_cast <const char*>(&Cmd_RevBuff[Cmd_RevBuff_Pos]), Cmd_RevBuff_Pos);


		std::string szLocalFilePath    =ParamLocalPath+ParamFileName;

		//写入文件
		if(nFileCurrIndex == 0)
		{
			//如果是初始包，则删除当前文件。
			remove(szLocalFilePath.c_str());
		}

		FILE* pFile = NULL;
		fopen_s(&pFile, szLocalFilePath.c_str(), "ab+");
		if(pFile == NULL)
		{
            logError("打开写入文件失败!!!");
			delete[] pBuffer;
			return false;
		}
		if (Cmd_RevBuff)  delete Cmd_RevBuff;

		fwrite(pBuffer, sizeof(char), nFileCurrBufferSize, pFile);
		fclose(pFile);
		delete[] pBuffer;
	}
	else
	{
        logError("获取远程文件失败!!!");
		return false;
	}

	return true;
}



bool CNetOperation::OpenLocalFile(const std::string& ParamLocalPath,  const std::string& ParamFileName)  //打开需要发送的本地文件
{
    localfile.open(ParamLocalPath+ParamFileName,std::iostream::in|std::iostream::binary|std::iostream::ate);
    
    LocalFileSize   =localfile.tellg(); 

    FileBlockSize  =MAX_BUFF_10240;

	if(LocalFileSize % FileBlockSize != 0)
	{
		FileBlockCount     = LocalFileSize / FileBlockSize + 1;
		LastBlockSize = LocalFileSize % FileBlockSize;
	}
	else
	{
		FileBlockCount     = LocalFileSize / FileBlockSize;
		LastBlockSize = FileBlockSize;
	}
	return true;
}    

void CNetOperation::closeLocalFile()
{
    localfile.close();        
}

long int  CNetOperation::GetFileSize()
{    
    return  LocalFileSize;        
}
     
int  CNetOperation::GetFileBlockCount(const int &ParamBufferSize)
{
    return  FileBlockCount;          
}
   
int  CNetOperation::GetFileLastBlockSize(const int &ParamBufferSize)
{
    return  LastBlockSize;       
}
   
void  CNetOperation::ReadFileBlock(const int& ParamBlockIndex, char* ParamFileBuffer)
{

		if(ParamBlockIndex != FileBlockCount - 1)
		{
			//如果不是最后一个块            
            localfile.seekg( (long)(ParamBlockIndex * FileBlockSize) );
            localfile.read( ParamFileBuffer, FileBlockSize );            		
        }
		else
		{
			//如果是最后一个块
            localfile.seekg( (long)(ParamBlockIndex * FileBlockSize) );
            localfile.read( ParamFileBuffer, LastBlockSize );    
		}   
    
}
        


bool CNetOperation::Send_Upload(const std::string  &ParamSvrUserName,const std::string  &ParamRemotePath ,const std::string  &ParamLocalPath, const std::string  &ParamFileName )
{

    for(int i = 0; i < FileBlockCount; i++)
	{
        Cmd_Packet.CmdVersion   =1;
        Cmd_Packet.CmdBuffPos   =0;
        Cmd_Packet.CmdID        = COMMAND_FTP_FILE_DOWNLOAD;    
        Cmd_Packet.CmdBuffPos   =0;
        sprintf_s(Cmd_Packet.CmdSession, 32, "FREEEYES");

        this->ReadFileBlock(i,this->FileBuffer);
        
        Cmd_Packet.SetCmdHead();
        Cmd_Packet.SetCmdPayload( ParamSvrUserName );
        Cmd_Packet.SetCmdPayload<short>( ParamRemotePath );
         
		if(i != FileBlockCount - 1)
		{
			//如果不是最后一个块
            ////CmdPacket    =Cmd_Packet.SetCmdHead(sizeof(char), ParamSvrUserName.c_str(),sizeof(short),ParamRemotePath);
            
            Cmd_Packet.SetCmdPayload<int>( FileBlockSize );
            Cmd_Packet.SetCmdPayload<int>( i );
            Cmd_Packet.SetCmdPayload<int>( FileBlockSize );
            Cmd_Packet.SetCmdPayload( FileBlockSize,this->FileBuffer );
		}
		else
		{
			//如果是最后一个块
           // CmdPacket    =Cmd_Packet.SetCmdHead(sizeof(char), ParamSvrUserName.c_str(),sizeof(short),ParamRemotePath);
            
            Cmd_Packet.SetCmdPayload<int>( LastBlockSize );
            Cmd_Packet.SetCmdPayload<int>( i );
            Cmd_Packet.SetCmdPayload<int>( LastBlockSize );
            Cmd_Packet.SetCmdPayload( LastBlockSize,this->FileBuffer );
      
		}

        
              
        Cmd_Packet.SetCmdPayloadLen();
        
        if(IsValidSocketHandle(m_sckClient->GetHandle()) )
        {
            return false;
        };
        
        _LoginInfo objLoginInfo;
        const   char * CmdPacket    =Cmd_Packet.CmdBuff;   
        this->SendPacket(objLoginInfo,CmdPacket,Cmd_Packet.CmdBuffPos);
           
        
        
        
        int RetPacketLength =0;
        this->GetRecvPktLength(RetPacketLength);

        //Cmd_Packet.CmdBuffPos   =0;
		char *Cmd_RevBuff = new char[RetPacketLength];

        this->RecvPacket(objLoginInfo,Cmd_RevBuff,RetPacketLength);   //接收报文
        
        short RecvCommandID = 0;
        int CmdRet           = 0;    
        this->GetCmdRetInfo(Cmd_RevBuff, RecvCommandID,CmdRet);   //获得接收到的命令ID以及执行返回结果
                
            
        if(CmdRet == OP_OK)
		{
			int dwError = GetLastSocketError();
			logError("远程服务器接收数据失败!!!");
			return false;
		}

		if (Cmd_RevBuff)  delete Cmd_RevBuff;
	}

    logInfo("上传文件成功!!!");

	return true;
}





bool CNetOperation::Send_UserInfo(const int &ParamUserID ,int &ParamReUserID,int &ParamLife,int &ParamMagic)
{    
    Cmd_Packet.CmdVersion   =1;
    Cmd_Packet.CmdBuffPos   =0;
    Cmd_Packet.CmdID        = COMMAND_LOGINCLIENT_USERINFO;    
    Cmd_Packet.CmdBuffPos   =0;
	sprintf_s(Cmd_Packet.CmdSession, 32, "FREEEYES");

    Cmd_Packet.SetCmdHead();
    Cmd_Packet.SetCmdPayload<int>( ParamUserID );   
    Cmd_Packet.SetCmdPayloadLen();
    
    if(IsValidSocketHandle(m_sckClient->GetHandle()) )
    {
        return false;
    };
    
    _LoginInfo objLoginInfo;
    const   char * CmdPacket    =Cmd_Packet.CmdBuff;   
    this->SendPacket(objLoginInfo,CmdPacket,Cmd_Packet.CmdBuffPos);
        
        
    int RetPacketLength =0;
    this->GetRecvPktLength(RetPacketLength);

    //Cmd_Packet.CmdBuffPos   =0;
	char *Cmd_RevBuff = new char[RetPacketLength];

    int  Cmd_RevBuff_Pos    =0;  
    this->RecvPacket(objLoginInfo,Cmd_RevBuff,RetPacketLength);   //接收报文
    
	short RecvCommandID = 0;
	int CmdRet           = 0;    
    this->GetCmdRetInfo(Cmd_RevBuff, RecvCommandID,CmdRet);   //获得接收到的命令ID以及执行返回结果
    Cmd_RevBuff_Pos =sizeof(short)+sizeof(int);
        
    GetBuffValue(reinterpret_cast <const char*>(&ParamReUserID),  sizeof(int), reinterpret_cast <const char*>(&Cmd_RevBuff[Cmd_RevBuff_Pos]), Cmd_RevBuff_Pos);
    GetBuffValue(reinterpret_cast <const char*>(&ParamLife),  sizeof(int), reinterpret_cast <const char*>(&Cmd_RevBuff[Cmd_RevBuff_Pos]), Cmd_RevBuff_Pos);
    GetBuffValue(reinterpret_cast <const char*>(&ParamMagic),  sizeof(int), reinterpret_cast <const char*>(&Cmd_RevBuff[Cmd_RevBuff_Pos]), Cmd_RevBuff_Pos);

	if (Cmd_RevBuff)  delete Cmd_RevBuff;	
	
	return true;
}

bool CNetOperation::Send_SetUserInfo( int nUserID, int nLife, int nMagic )
{

    Cmd_Packet.CmdVersion   =1;
    Cmd_Packet.CmdBuffPos   =0;
    Cmd_Packet.CmdID        = COMMAND_LOGINCLIENT_SET_USERINFO;    
    Cmd_Packet.CmdBuffPos   =0;
	sprintf_s(Cmd_Packet.CmdSession, 32, "FREEEYES");

  
    Cmd_Packet.SetCmdHead();
    Cmd_Packet.SetCmdPayload<int>( nUserID );
    Cmd_Packet.SetCmdPayload<int>( nLife ); 
    Cmd_Packet.SetCmdPayload<int>( nMagic );             
    Cmd_Packet.SetCmdPayloadLen();
    
    if(IsValidSocketHandle(m_sckClient->GetHandle()) )
    {
        return false;
    };
    
    _LoginInfo objLoginInfo;
    const   char * CmdPacket    =Cmd_Packet.CmdBuff;   
    this->SendPacket(objLoginInfo,CmdPacket,Cmd_Packet.CmdBuffPos);
       
    
    
    int RetPacketLength =0;
    this->GetRecvPktLength(RetPacketLength);

	char *Cmd_RevBuff = new char[RetPacketLength];

    int  Cmd_RevBuff_Pos    =0;   
    this->RecvPacket(objLoginInfo,Cmd_RevBuff,RetPacketLength);   //接收报文
    
	short RecvCommandID = 0;
	int CmdRet           = 0;    
    this->GetCmdRetInfo(Cmd_RevBuff, RecvCommandID,CmdRet);   //获得接收到的命令ID以及执行返回结果
    Cmd_RevBuff_Pos =sizeof(short)+sizeof(int);
      
	if (Cmd_RevBuff)  delete Cmd_RevBuff;  

	if(CmdRet == LOGIN_SUCCESS)
	{
        logInfo("设置用户信息成功!!!");
	}
	else
	{
        logError("设置用户信息失败!!!");
	}

	return true;
}




bool CNetOperation::Send_Plug(const char* ParamText ,short &ParamReCmdID,int &ParamRetB,int &ParamRetA)
{
        
    Cmd_Packet.CmdVersion   =1;
    Cmd_Packet.CmdBuffPos   =0;
    Cmd_Packet.CmdID        = COMMAND_PLUGA;    
    Cmd_Packet.CmdBuffPos   =0;
	sprintf_s(Cmd_Packet.CmdSession, 32, "FREEEYES");

    Cmd_Packet.SetCmdHead();
    Cmd_Packet.SetCmdPayload<short>( ParamText );  
    Cmd_Packet.SetCmdPayloadLen();
    
    if(IsValidSocketHandle(m_sckClient->GetHandle()) )
    {
        return false;
    };
    
    _LoginInfo objLoginInfo;
    const   char * CmdPacket    =Cmd_Packet.CmdBuff;   
    this->SendPacket(objLoginInfo,CmdPacket,Cmd_Packet.CmdBuffPos);
        
        
    int RetPacketLength =0;
    this->GetRecvPktLength(RetPacketLength);

    //Cmd_Packet.CmdBuffPos   =0;
	char *Cmd_RevBuff = new char[RetPacketLength];

    int  Cmd_RevBuff_Pos    =0;  
    this->RecvPacket(objLoginInfo,Cmd_RevBuff,RetPacketLength);   //接收报文
    
    this->GetCmdRetInfo(Cmd_RevBuff, ParamReCmdID,ParamRetB);   //获得接收到的命令ID以及执行返回结果
    Cmd_RevBuff_Pos =sizeof(short)+sizeof(int);
        
    GetBuffValue(reinterpret_cast <const char*>(&ParamRetA),  sizeof(int), reinterpret_cast <const char*>(&Cmd_RevBuff[Cmd_RevBuff_Pos]), Cmd_RevBuff_Pos);
	
	if (Cmd_RevBuff)  delete Cmd_RevBuff;
    return true;
}
