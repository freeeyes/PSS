#include "LinuxCPU.h"

void trim(string& s)
{
    int index = 0;

    if (!s.empty())
    {
        while ((index = (int)s.find(' ', index)) != (int)string::npos)
        {
            s.erase(index, 1);
        }
    }

    //去掉前面的09和后面的KB,只得到数字部分
    s.erase(0, s.find_first_not_of(0x09));
    s.erase(s.find_last_not_of("kB") + 1);
}

string GetFileContent(const string& strFileName)
{
    string strContent;
#if PSS_PLATFORM != PLATFORM_WIN
    std::ifstream in(strFileName.c_str(), std::ios::in);
    std::stringstream buf;
    buf << in.rdbuf();
    strContent = buf.str();
    in.close();
#endif
    return strContent;
}

int32 GetProcessCPU_Idel_Linux()
{
    int32 nRet = 0;
    string strcmd = "ps -aux | grep ";
    ACE_TString strpid;

    std::stringstream ss_format;
    ss_format << ACE_OS::getpid();
    strcmd += ss_format.str();

    string strCPU = strcmd;
    strCPU += "  |awk '{print $2,$3}' >> aasnowy.txt";
    nRet = system(strCPU.c_str());   //获取CPU命令

    if (nRet == -1)
    {
        return 0;
    }

    string strContent = GetFileContent("aasnowy.txt");

    //切分出CPU数据
    bool blFlag = false;
    auto nLen = (int32)strContent.length();
    int32 i = 0;

    for (i = 0; i < nLen; i++)
    {
        if (strContent[i] == ' ')
        {
            blFlag = true;
            break;
        }
    }

    string strTmp;

    if (blFlag == true)
    {
        strTmp.append(&strContent[i], (size_t)(nLen - i));
    }

    float fcpu;
    fcpu = (float)atof(strTmp.c_str());

    strCPU = "rm -rf aasnowy.txt";
    nRet = system(strCPU.c_str());

    if (nRet == -1)
    {
        return 0;
    }
    else
    {
        return (int32)(fcpu * 100);
    }
}

int32 GetProcessMemorySize_Linux()
{
    int nMomorySize = 0;
#if PSS_PLATFORM != PLATFORM_WIN
    string strFileName;

    std::stringstream ss_format;
    ss_format << "//proc//" << ACE_OS::getpid() << "//status";
    strFileName = ss_format.str();

    string strContent = GetFileContent(strFileName);

    int vmrss = 0;
    int vmsize = 0;

    auto lines = split_string(strContent, '\n');

    PSS_LOGGER_DEBUG("[strContent]lines={0}.", lines.size());

    for (const auto& strline : lines)
    {
        auto memory_info = split_string(strline, ':');

        if (memory_info.size() == 2 && memory_info[0] == "VmRSS")
        {
            string strValue = memory_info[1].c_str();
            trim(strValue);
            vmrss = atoi(strValue.c_str()) / 1024;
        }
        else if (memory_info.size() == 2 && memory_info[0] == "VmSize")
        {
            string strValue = memory_info[1].c_str();
            trim(strValue);
            vmsize = atoi(strValue.c_str()) / 1024;
        }
    }

    nMomorySize = vmrss + vmsize;
#endif
    return nMomorySize;
}
