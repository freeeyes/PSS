#ifndef __GBK_TO_UTF8_FILE_H__
#define __GBK_TO_UTF8_FILE_H__

#include<string>
#include<iostream>
using namespace std;

/**GBK和UTF-8的字符串转换*/
string ConvertGBKToUtf8(const string& strGBK);
string ConvertUtf8ToGBK(const string& strUtf8);

/**检测字符串是否是utf8编码*/
bool IsTextUTF8(const char* str, int length);
bool IsTextUTF8(const string& str);

/**计算字符串的二进制表示*/
string PrintStringAsBinaryString(const char* p);
string PrintStringAsBinaryString(const string& str);

/**Unicode和UTF-8的编码转换*/
unsigned short one_gbk_to_unicode(unsigned char ch, unsigned char cl);
unsigned short one_unicode_to_gbk(unsigned short unicode); //not implement
void unicode_to_gbk(unsigned short* punicode, char* pgbk, int len);
int one_unicode_to_utf8(unsigned long unic, unsigned char* pOutput, int outSize);

//参数1是UTF8字符串当前位置指针，这里必须要是指针，因为必须要通过第1个字符进行判断才知道一个完整的字符的编码要向后取多少个字符
//参数2是返回的UCS-2编码的Unicode字符
int one_utf8_to_unicode(const char* utf8, unsigned short& wch);
//参数1是UTF8编码的字符串
//参数2是输出的UCS-2的Unicode字符串
//参数3是参数1字符串的长度
//使用的时候需要注意参数2所指向的内存块足够用。其实安全的办法是判断一下pUniBuf是否为NULL，如果为NULL则只统计输出长度不写pUniBuf，这样
//通过两次函数调用就可以计算出实际所需要的Unicode缓存输出长度。当然，更简单的思路是：无论如何转换，UTF8的字符数量不可能比Unicode少，所
//以可以简单的按照sizeof(wchar_t) * utf8Leng来分配pUniBuf的内存……
int utf8_to_unicode(const char* utf8Buf, unsigned short* pUniBuf, int utf8Leng);
int unicode_to_utf8(unsigned short wchar, char* utf8);

#endif //__GBK_TO_UTF8_FILE_H__