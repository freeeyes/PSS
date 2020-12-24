#ifndef _IPCHECK_H
#define _IPCHECK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <vector>
#include <string>

using namespace std;

enum EM_CHECK_IP_TYPE
{
    IP_V4 = 0,
    IP_V6,
    IP_UNKNOW,
};

static void str_split(const string& src, const string& separator, vector<string>& dest)
{
    string str = src;
    string substring;
    string::size_type start = 0, index;

    do
    {
        index = str.find(separator, start);

        if (index != string::npos)
        {
            substring = str.substr(start, index - start);
            dest.push_back(substring);
            start = str.find_first_not_of(separator, index);

            if (start == string::npos)
            {
                return;
            }
        }
    }
    while (index != string::npos);

    //the last token
    substring = str.substr(start);
    dest.push_back(substring);
}

static EM_CHECK_IP_TYPE Check_IP_V6(vector<string> vecIpSection)
{
    for (int i = 0; i < (int)vecIpSection.size(); i++)
    {
        if (vecIpSection[i].c_str() == (string)"")
        {
            continue;
        }

        char* pStop = nullptr;
        long int nSection = (long int)strtol(vecIpSection[i].c_str(), &pStop, 16);

        int nDataLength = (int)(pStop - vecIpSection[i].c_str());

        if (nDataLength != (int)vecIpSection[i].length())
        {
            return IP_UNKNOW;
        }

        if (nSection < 0x0000 || nSection > 0xffff)
        {
            return IP_UNKNOW;
        }
    }

    return IP_V6;
}

static EM_CHECK_IP_TYPE Check_IP_V4(vector<string> vecIpSection)
{
    if (vecIpSection.size() != 4)
    {
        return IP_UNKNOW;
    }


    for (int i = 0; i < (int)vecIpSection.size(); i++)
    {
        char* pStop = nullptr;
        long int nSection = (long int)strtol(vecIpSection[i].c_str(), &pStop, 10);

        int nDataLength = (int)(pStop - vecIpSection[i].c_str());

        if (nDataLength != (int)vecIpSection[i].length())
        {
            return IP_UNKNOW;
        }

        if (nSection < 0 || nSection > 255)
        {
            return IP_UNKNOW;
        }
    }

    return IP_V4;
}

inline EM_CHECK_IP_TYPE Check_IP(string strIP)
{
    if (strIP == "INADDR_ANY")
    {
        return IP_V4;
    }

    vector<string> vecIpSection;

    str_split(strIP, ".", vecIpSection);

    if (vecIpSection.size() > 1)
    {
        return Check_IP_V4(vecIpSection);
    }

    vecIpSection.clear();
    str_split(strIP, "::", vecIpSection);

    if (vecIpSection.size() > 2)
    {
        return IP_UNKNOW;
    }

    if (vecIpSection.size() == 1)
    {
        vecIpSection.clear();
        str_split(strIP, ":", vecIpSection);

        if (vecIpSection.size() == 8)
        {
            return Check_IP_V6(vecIpSection);
        }
    }
    else
    {
        for (int i = 0; i < (int)vecIpSection.size(); i++)
        {
            if (vecIpSection[i].length() == 0)
            {
                continue;
            }
            else
            {
                vector<string> vecIpSubSection;
                str_split(vecIpSection[i], ":", vecIpSubSection);

                if (IP_UNKNOW == Check_IP_V6(vecIpSubSection))
                {
                    return IP_UNKNOW;
                }
            }
        }

        return IP_V6;
    }

    return IP_UNKNOW;
}

inline string Get_Type_Name(EM_CHECK_IP_TYPE emType)
{
    string strTypeName;

    if (IP_V4 == emType)
    {
        strTypeName = "IPV4";
    }
    else if (IP_V6 == emType)
    {
        strTypeName = "IPV6";
    }
    else
    {
        strTypeName = "IP Unknow";
    }

    return strTypeName;
}


/*
根据ip地址，判断Ip类型
参数: in  ip: 传入的Ip地址
        out ipType: 需要写入的类型
返回值: false为非法ip类型，ture为正常ip类型.
*/
inline bool Check_IPType(const std::string& ip, uint8& ipType)
{
    bool bKet = true;
    EM_CHECK_IP_TYPE emIpType = Check_IP(ip);

    if (IP_UNKNOW == emIpType)
    {
        PSS_LOGGER_DEBUG("[XmlConfig::SetIPType]({0}) IP is {1}.",
                   ip,
                   Get_Type_Name(emIpType));
        bKet = false;
    }
    else if (IP_V6 == emIpType)
    {
        ipType = TYPE_IPV6;
    }
    else
    {
        ipType = TYPE_IPV4;
    }

    return bKet;
}

#endif
