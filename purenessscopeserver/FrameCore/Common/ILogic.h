#pragma once
#ifndef ILOGIC_H
#define ILOGIC_H

#include "define.h"
#include <string>

//add 与工作线程无关的业务消息队列的逻辑业务类,必须继承后实现 liuruiqi

class ILogic
{
public:
	//初始化必填 参数:逻辑ID, 描述, 检测时间(秒)
	ILogic(uint32 nLogicID, const char* descriptor, uint32 nTimeout);
	virtual ~ILogic();
public:
	//获取线程ID
	uint32 GetLogicThreadID();
	//超时
	void SetTimeOut(uint32 nTimeout);
	uint32 GetTimeOut();
	//描述
	std::string GetDescriptor();
public:
	//初始化内容
	virtual bool Init() = 0;
	//执行逻辑
	virtual uint32 Run() = 0;
	//错误处理
	virtual uint32 Error() = 0;
	//退出善后
	virtual void Exit() = 0;
private:
	uint32 uLogicThreadID;
	uint32 uTimeout;
	std::string des;
};


#endif // ILOGIC_H