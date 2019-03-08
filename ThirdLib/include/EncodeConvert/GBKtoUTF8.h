#ifndef __GBK_TO_UTF8_FILE_H__
#define __GBK_TO_UTF8_FILE_H__

#include<string>
#include<iostream>
using namespace std;

/**GBK��UTF-8���ַ���ת��*/
string ConvertGBKToUtf8(const string& strGBK);
string ConvertUtf8ToGBK(const string& strUtf8);

/**����ַ����Ƿ���utf8����*/
bool IsTextUTF8(const char* str, int length);
bool IsTextUTF8(const string& str);

/**�����ַ����Ķ����Ʊ�ʾ*/
string PrintStringAsBinaryString(const char* p);
string PrintStringAsBinaryString(const string& str);

/**Unicode��UTF-8�ı���ת��*/
unsigned short one_gbk_to_unicode(unsigned char ch, unsigned char cl);
unsigned short one_unicode_to_gbk(unsigned short unicode); //not implement
void unicode_to_gbk(unsigned short* punicode, char* pgbk, int len);
int one_unicode_to_utf8(unsigned long unic, unsigned char* pOutput, int outSize);

//����1��UTF8�ַ�����ǰλ��ָ�룬�������Ҫ��ָ�룬��Ϊ����Ҫͨ����1���ַ������жϲ�֪��һ���������ַ��ı���Ҫ���ȡ���ٸ��ַ�
//����2�Ƿ��ص�UCS-2�����Unicode�ַ�
int one_utf8_to_unicode(const char* utf8, unsigned short& wch);
//����1��UTF8������ַ���
//����2�������UCS-2��Unicode�ַ���
//����3�ǲ���1�ַ����ĳ���
//ʹ�õ�ʱ����Ҫע�����2��ָ����ڴ���㹻�á���ʵ��ȫ�İ취���ж�һ��pUniBuf�Ƿ�ΪNULL�����ΪNULL��ֻͳ��������Ȳ�дpUniBuf������
//ͨ�����κ������þͿ��Լ����ʵ������Ҫ��Unicode����������ȡ���Ȼ�����򵥵�˼·�ǣ��������ת����UTF8���ַ����������ܱ�Unicode�٣���
//�Կ��Լ򵥵İ���sizeof(wchar_t) * utf8Leng������pUniBuf���ڴ桭��
int utf8_to_unicode(const char* utf8Buf, unsigned short* pUniBuf, int utf8Leng);
int unicode_to_utf8(unsigned short wchar, char* utf8);

#endif //__GBK_TO_UTF8_FILE_H__