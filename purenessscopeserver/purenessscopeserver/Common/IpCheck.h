#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <vector>
#include <string>

using namespace std;

enum EM_IP_TYPE
{
	IP_V4 = 0,
	IP_V6,
	IP_UNKNOW,
};

static void str_split(const string &str, const string &sign, vector<string> &results)
{
    string::size_type pos;
    size_t size = str.size();
    for (size_t i = 0; i < size; ++i)
    {
        pos = str.find(sign, i); 
        if (pos == str.npos)
        {
            string s = str.substr(i, size);
            results.push_back(s);
            break;
        }
        if (pos < size)
        {
            string s = str.substr(i, pos - i);
            results.push_back(s);
            i = pos;
        }
    }
}

static EM_IP_TYPE Check_IP_V6(vector<string> vecIpSection)
{
	printf("[Check_IP_V6]size=%d.\n", vecIpSection.size());
	if(vecIpSection.size() != 8)
	{
		return IP_UNKNOW;
	}
	
	//判断每个IP段的合法性
	for(int i = 0; i < vecIpSection.size(); i++)
	{
		if(vecIpSection[i].c_str() == "")
		{
			continue;
		}
		
		char* pStop = NULL;
		long int nSection = (long int)strtol(vecIpSection[i].c_str(), &pStop, 16);
		
		if((int)(pStop - vecIpSection[i].c_str()) != vecIpSection[i].length())
		{
			printf("[Check_IP_V4]error Section[%s].\n", vecIpSection[i].c_str());
			return IP_UNKNOW;
		}
		
		//printf("[Check_IP_V4]nSection=%d.\n", nSection);
		if(nSection < 0x0000 || nSection > 0xffff)
		{
			return IP_UNKNOW;
		}
	}
	
	return IP_V6;
}

static EM_IP_TYPE Check_IP_V4(vector<string> vecIpSection)
{
	//printf("[Check_IP_V4]size=%d.\n", vecIpSection.size());
	if(vecIpSection.size() != 4)
	{
		return IP_UNKNOW;
	}
	
	//判断每个IP段的合法性
	for(int i = 0; i < vecIpSection.size(); i++)
	{
		char* pStop = NULL;
		long int nSection = (long int)strtol(vecIpSection[i].c_str(), &pStop, 10);
		
		if((int)(pStop - vecIpSection[i].c_str()) != vecIpSection[i].length())
		{
			printf("[Check_IP_V4]error Section[%s].\n", vecIpSection[i].c_str());
			return IP_UNKNOW;
		}
		
		//printf("[Check_IP_V4]nSection=%d.\n", nSection);
		if(nSection < 0 || nSection > 255)
		{
			return IP_UNKNOW;
		}
	}
	
	return IP_V4; 
}

static EM_IP_TYPE Check_IP(string strIP)
{
	//判断是IPv4还是IPv6格式
	vector<string> vecIpSection;
	
	str_split(strIP, ".", vecIpSection);
	//printf("[Check_IP]v4 vecIpSection size=%d.\n", vecIpSection.size());
	if(vecIpSection.size() > 1)
	{
		return Check_IP_V4(vecIpSection);
	}
	
	vecIpSection.clear();
	str_split(strIP, ":", vecIpSection);
	//printf("[Check_IP]v6 vecIpSection size=%d.\n", vecIpSection.size());
	if(vecIpSection.size() > 1)
	{
		return Check_IP_V6(vecIpSection);
	}
	
	return IP_UNKNOW;
}

//将对应的IP类型转换成字符串输出
static string Get_Type_Name(EM_IP_TYPE emType)
{
	if(IP_V4 == emType)
	{
		return "IPV4";
	}
	else if(IP_V6 == emType)
	{
		return "IPV6";
	}
	else
	{
		return "IP Unknow";
	}
}
