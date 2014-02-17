/*
 * 信令定义，以及一些基本方法
 * by w1w
 */

#pragma once

#ifndef _COMMANDDEFINE_H
#define _COMMANDDEFINE_H

//定义客户端信令(TCP)
#define COMMAND_REG    0x1000                    //注册APP客户端
#define COMMAND_REG_RESPONSE    0x1001           //注册结果回应包

#define COMMAND_KEEP_ALIVE    0x2000             //心跳
#define COMMAND_KEEP_ALIVE_RESPONSE    0x2001    //心跳回应包
#define COMMAND_QUERY_APP    0x2010              //查询当前在线的APP
#define COMMAND_QUERY_APP_RESPONSE    0x2011     //查询APP回应包
#define COMMAND_SYN_APP    0x2012                // 更新APP状态包
#define COMMAND_QUERY_SERVER    0x2020           //查询服务器状态
#define COMMAND_QUERY_SERVER_RESPONSE    0x2021  //查询服务器回应包

#define COMMAND_PACKET    0x3000                 //数据包
#define COMMAND_PACKET_RECEIVED    0x3001        //数据包正确接受
#define COMMAND_PACKET_SERVERDOWN    0x3002      //服务器不在线
#define COMMAND_PACKET_APPDOWN    0x3003         //APP不在线
#define COMMAND_PACKET_REJECT    0x3004          //拒绝接收
#define COMMAND_PACKET_ERR    0x3005             //数据包验证错误
#define COMMAND_PACKET_OUTTIME    0x3006         //数据包超时

//初始化字符串为全0
inline void iniChar(char *chars,int l)
{
	for (int i = 0; i<l;++i)
	{
		chars[i]='\0';
	}
}


#endif