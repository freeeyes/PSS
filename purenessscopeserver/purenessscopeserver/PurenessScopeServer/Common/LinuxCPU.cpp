#include "LinuxCPU.h"

int32 GetProcessCPU_Idel_Linux()
{
    int32 nRet = 0;
    ACE_TString strcmd ="ps -aux | grep ";
    char szbuffid[MAX_BUFF_20];
    ACE_TString strpid;
    sprintf_safe(szbuffid, MAX_BUFF_20, "%d", ACE_OS::getpid());
    strpid = szbuffid;
    strcmd += strpid;
    ACE_TString strCPU = strcmd;
    strCPU +="  |awk '{print $2,$3}' >> aasnowy.txt";
    nRet = system(strCPU.c_str());   //获取CPU命令

    if (nRet == -1)
    {
        return 0;
    }

    char szbuffer[50];

    FILE* fd = ACE_OS::fopen("aasnowy.txt","r");

    if (NULL == fd)
    {
        return -1;
    }

    char* pReturn = fgets(szbuffer,sizeof(szbuffer),fd);

    if (NULL == pReturn)
    {
        fclose(fd);
        return -1;
    }

    //切分出CPU数据
    bool blFlag = false;
    int32 nLen = (int32)ACE_OS::strlen(szbuffer);
    int32 i = 0;

    for(i = 0; i < nLen; i++)
    {
        if(szbuffer[i] == ' ')
        {
            blFlag = true;
            break;
        }
    }

    char szTmp[50] = {'\0'};

    if(blFlag == true)
    {
        memcpy_safe(&szbuffer[i], (uint32)nLen - i, szTmp, (uint32)50);
        szTmp[nLen - i] = '\0';
    }

    float fcpu;
    fcpu = (float)atof(szTmp);
    fclose(fd);
    nRet = system("rm -rf aasnowy.txt");

    return (int32)(fcpu*100);
}

int32 GetProcessMemorySize_Linux()
{
    int32 nRet = 0;
    ACE_TString strcmd ="ps -aux | grep ";
    char szbuffid[MAX_BUFF_20];
    ACE_TString strpid;
    sprintf_safe(szbuffid, MAX_BUFF_20, "%d", ACE_OS::getpid());
    strpid = szbuffid;
    strcmd += strpid;
    ACE_TString strMem = strcmd;
    strMem +="  |awk '{print $2,$6}' >> aasnowy.txt";
    nRet = system(strMem.c_str()); //获取内存命令

    if (-1 == nRet)
    {
        return -1;
    }

    char szbuffer[50];
    FILE* fd = ACE_OS::fopen("aasnowy.txt","r");

    if (NULL == fd)
    {
        return -1;
    }

    char* pReturn = fgets(szbuffer,sizeof(szbuffer),fd);

    if (NULL == pReturn)
    {
        fclose(fd);
        return -1;
    }

    //切分出内存数据
    bool blFlag = false;
    int32 nLen = (int32)ACE_OS::strlen(szbuffer);
    int32 i = 0;

    for(i = 0; i < nLen; i++)
    {
        if(szbuffer[i] == ' ')
        {
            blFlag = true;
            break;
        }
    }

    char szTmp[50] = {'\0'};

    if(blFlag == true)
    {
        memcpy_safe(&szbuffer[i], (uint32)nLen - i, szTmp, (uint32)50);
        szTmp[nLen - i] = '\0';
    }

    int32 nMem = 0;
    nMem = atoi(szTmp);
    fclose(fd);
    nRet = system("rm -rf aasnowy.txt");

    return nMem * 1000;
}
