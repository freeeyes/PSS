#include "Mylog.h"


CLog2File* CLog2File::pInstance = NULL;

CLog2File& CLog2File::Initialize( )
{
	if (NULL == pInstance)
	{
		pInstance = new CLog2File();
		//pInstance->InitLogFile(strLogFile);
	}
	return *pInstance;
}


CLog2File::CLog2File()
{

}

CLog2File::~CLog2File(void)
{
}


bool CLog2File::InitLogFile(const std::string &szFileName)    
{
    m_FileName  =szFileName;
	m_LogFile.open(m_FileName.c_str(), ios_base::out | ios_base::app);
 	if (!m_LogFile)
	{
		return false;
	}

	return true;
}


void CLog2File::CloseLogFile()    
{
	if (m_LogFile.is_open())
	{
		m_LogFile.close();
	}
}


void CLog2File::WriteLog( const std::string &x)
{
	if (m_LogFile.is_open())
	{
        m_LogFile.seekp(ios::end);
        m_LogFile <<  x <<endl;
	}

}

