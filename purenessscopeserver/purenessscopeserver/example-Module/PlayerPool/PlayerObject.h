#ifndef _PLAYEROBJECT_H
#define _PLAYEROBJECT_H

#include <stdio.h>
#include <time.h>

#include "define.h"

//数据结构体的基类
class CObject
{
public:
	CObject() { m_blWrite = false; m_ttUpdateTime = time(NULL); };
	virtual ~CObject() {};

	void EnterWrite() { m_blWrite = true;}
	void LeavelWrite() { m_ttUpdateTime = time(NULL); m_blWrite = false;}

	bool GetWriteSate() { return m_blWrite; }

#define ENTERWRITE() EnterWrite();    //定义写入的宏
#define LEAVELWEITE() LeavelWrite();  //定义写完的宏

private:
	bool   m_blWrite;      //写标记

public:
	time_t m_ttUpdateTime; //数据更新时间，DS服务器会更具这个时间来决定是否更新。
};

//数据结构，继承于CDataBase，用于实现数据的详细内部结构
class CPlayerData : public CObject
{
public:
	CPlayerData() 
	{ 
		m_blOnline  = false;
		m_nPlayerID = 0;
		m_nLevel    = 1;
	};

	~CPlayerData() {}

	void Create(const char* pPlayerName)
	{
		ENTERWRITE();  //标记写标记

		sprintf_safe(m_szPlayerName, 50, "%s", pPlayerName);
		m_nPlayerID = 0;
		m_nLevel    = 1;

		LEAVELWEITE();  //释放写标记
	};

	const char* GetPlayerName()
	{
		return m_szPlayerName;
	};

	void SetPlayerName(const char* szPlayerName)
	{
		sprintf_safe(m_szPlayerName, 50, "%s", szPlayerName);
	}

	int GetLevel()
	{
		return m_nLevel;
	};

	void SetPlayerLevel(int nLevel)
	{
		m_nLevel = nLevel;
	}

	int GetPlayerID()
	{
		return m_nPlayerID;
	};

	void SetPlayerID(int nID)
	{
		m_nPlayerID = nID;
	}

	const char* GetKey()
	{
		return m_szPlayerName;
	};

	const time_t GetTimeStamp()
	{
		return m_ttUpdateTime;
	};

	void SetTimeStamp()
	{
		m_ttUpdateTime = time(NULL);
	}

	void SetOnline(bool blOnline)
	{
		m_blOnline = blOnline;
		m_ttUpdateTime = time(NULL);
	};

	bool GetOnline()
	{
		return m_blOnline;
	};

private:
	char m_szPlayerName[50];     //玩家的名字
	int  m_nPlayerID;            //玩家的ID
	int  m_nLevel;               //玩家的等级
	bool m_blOnline;             //玩家是否在线
};

#endif
