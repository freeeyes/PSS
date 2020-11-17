#include "LinuxCPU.h"

int32 GetProcessCPU_Idel_Linux()
{
    int32 nRet = 0;
    string strcmd ="ps -aux | grep ";
    ACE_TString strpid;

    std::stringstream ss_format;
    ss_format << ACE_OS::getpid();
    strcmd += ss_format.str();

    string strCPU = strcmd;
    strCPU +="  |awk '{print $2,$3}' >> aasnowy.txt";
    nRet = system(strCPU.c_str());   //��ȡCPU����

    if (nRet == -1)
    {
        return 0;
    }

    char szbuffer[50];

    FILE* fd = ACE_OS::fopen("aasnowy.txt","r");

    if (nullptr == fd)
    {
        return -1;
    }

    const char* pReturn = ACE_OS::fgets(szbuffer,sizeof(szbuffer),fd);

    if (nullptr == pReturn)
    {
        fclose(fd);
        return -1;
    }

    //�зֳ�CPU����
    bool blFlag = false;
    auto nLen = (int32)ACE_OS::strlen(szbuffer);
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
        memcpy_safe(&szbuffer[i], (uint32)nLen - i, szTmp, (uint32)50, true);
    }

    float fcpu;
    fcpu = (float)atof(szTmp);
    fclose(fd);
    strCPU = "rm -rf aasnowy.txt";
    nRet = system(strCPU.c_str());

    if (nRet == -1)
    {
        return 0;
    }
    else
    {
        return (int32)(fcpu*100);
    }
}

int32 GetProcessMemorySize_Linux()
{
    int nMomorySize = 0;
#ifndef WIN32
    char file_name[64] = { 0 };
    FILE* fd;
    char line_buff[512] = { 0 };
    snprintf(file_name, 64, "/proc/%d/status", getpid());

    fd = fopen(file_name, "r");

    if (nullptr == fd)
    {
        return 0;
    }

    char name[64];
    int vmrss = 0;
    int vmsize = 0;

    while (fgets(line_buff, sizeof(line_buff), fd) != nullptr)
    {
        if (strncmp(line_buff, szVmRSS, strlen(szVmRSS)) == 0)
        {
            sscanf(line_buff, "%s %d", name, &vmrss);
            vmrss = vmrss / 1024;
        }

        if (strncmp(line_buff, szVmSize, strlen(szVmSize)) == 0)
        {
            sscanf(line_buff, "%s %d", name, &vmsize);
            vmsize = vmsize / 1024;
        }
    }

    fclose(fd);

    nMomorySize = vmrss + vmsize;
#endif
    return nMomorySize;
}
