#include "ITimerManager.h"   //--
#include "LuaFn.h"
 
CLuaFn::CLuaFn(void)
{
	m_pState = NULL;
	Init();
}

CLuaFn::~CLuaFn(void)
{
	Close();
}

void CLuaFn::Init()
{
	if(NULL == m_pState)
	{
		m_pState = lua_open();
		luaL_openlibs(m_pState);
	}
}

void CLuaFn::Close()
{
	if(NULL != m_pState)
	{
		lua_close(m_pState);
		m_pState = NULL;
	}
}

bool CLuaFn::LoadLuaFile(const char* pFileName)
{
	int nRet = 0;
	if(NULL == m_pState)
	{
		printf("[CLuaFn::LoadLuaFile]m_pState is NULL.\n");
		return false;
	}

	nRet = luaL_dofile(m_pState, pFileName);
	if (nRet != 0)
	{
		printf("[CLuaFn::LoadLuaFile]luaL_loadfile(%s) is file(%d)(%s).\n", pFileName, nRet, lua_tostring(m_pState, -1));
		return false;
	}
	lua_resume(m_pState, 0);

	return true;
}

bool CLuaFn::CallFileFn(const char* pFunctionName, CParamGroup& ParamIn, CParamGroup& ParamOut)
{
	int nRet = 0;
	int i    = 0;
	if(NULL == m_pState)
	{
		printf("[CLuaFn::CallFileFn]m_pState is NULL.\n");
		return false;
	}

	lua_getglobal(m_pState, pFunctionName);

	//加载输入参数
	for(i = 0; i < ParamIn.GetCount(); i++)
	{
		PushLuaData(m_pState, ParamIn.GetParam(i));
	}

	nRet = lua_pcall(m_pState, ParamIn.GetCount(), ParamOut.GetCount(), 0);
	if (nRet != 0)
	{
		printf("[CLuaFn::CallFileFn]call function(%s) error(%s).\n", pFunctionName, lua_tostring(m_pState, -1));
		return false;
	}

	//获得输出参数
	int nPos = 0;
	for(i = ParamOut.GetCount() - 1; i >= 0; i--)
	{
		nPos--;
		PopLuaData(m_pState, ParamOut.GetParam(i), nPos);
	}

	int nCount = lua_gettop(m_pState);
	lua_settop(m_pState, -1-ParamOut.GetCount());

	return true;
}

bool CLuaFn::InitClass()
{
	if(NULL == m_pState)
	{
		printf("[CLuaFn::InitClass]m_pState is NULL.\n");
		return false;
	}

	tolua_open(m_pState);
	tolua_module(m_pState, NULL, 0);
	tolua_beginmodule(m_pState, NULL);

	//需要注册的函数在这里
	//房间玩家相关API
	m_LuaFnRegister.AddFunc("LuaFn_Print", LuaFn_Print);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_Join", LuaFn_Room_API_Join);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_Exit", LuaFn_Room_API_Exit);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_Begin", LuaFn_Room_API_Begin);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_End", LuaFn_Room_API_End);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_GetPlayerTimerID", LuaFn_Room_API_GetPlayerTimerID);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_SetPlayerMoney", LuaFn_Room_API_SetPlayerMoney);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_GetPlayerMoney", LuaFn_Room_API_GetPlayerMoney);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_SetPlayerCard", LuaFn_Room_API_SetPlayerCard);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_GetPlayerCard", LuaFn_Room_API_GetPlayerCard);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_DelPalyerCard", LuaFn_Room_API_DelPalyerCard);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_GetPlayerCardCount", LuaFn_Room_API_GetPlayerCardCount);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_SetRoomTableCard", LuaFn_Room_API_SetRoomTableCard);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_GetRoomTableCard", LuaFn_Room_API_GetRoomTableCard);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_DelRoomTableCard", LuaFn_Room_API_DelRoomTableCard);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_SetRoomBankCard", LuaFn_Room_API_SetRoomBankCard);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_GetRoomBankCard", LuaFn_Room_API_GetRoomBankCard);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_DelRoomBankCard", LuaFn_Room_API_DelRoomBankCard);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_GetRoomPlayerName", LuaFn_Room_API_GetRoomPlayerName);
	m_LuaFnRegister.AddFunc("LuaFn_PlayerTimer_API_Add", LuaFn_PlayerTimer_API_Add);
	m_LuaFnRegister.AddFunc("LuaFn_PlayerTimer_API_Del", LuaFn_PlayerTimer_API_Del);
	m_LuaFnRegister.AddFunc("LuaFn_RoomTimer_API_Add", LuaFn_RoomTimer_API_Add);
	m_LuaFnRegister.AddFunc("LuaFn_RoomTimer_API_Del", LuaFn_RoomTimer_API_Add);
	m_LuaFnRegister.AddFunc("LuaFn_RoomName_API_Set", LuaFn_RoomName_API_Set);
	m_LuaFnRegister.AddFunc("LuaFn_RoomName_API_Get", LuaFn_RoomName_API_Get);
	m_LuaFnRegister.AddFunc("LuaFn_RoomDesc_API_Set", LuaFn_RoomDesc_API_Set);
	m_LuaFnRegister.AddFunc("LuaFn_RoomDesc_API_Get", LuaFn_RoomDesc_API_Get);
	m_LuaFnRegister.AddFunc("LuaFn_RoomPlayerLocation_API_Get", LuaFn_RoomPlayerLocation_API_Get);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_JoinLocation", LuaFn_Room_API_JoinLocation);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_PlayerSit", LuaFn_Room_API_PlayerSit);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_EnterRoom", LuaFn_Room_API_EnterRoom);
	m_LuaFnRegister.AddFunc("LuaFn_RoomTimer_API_Check", LuaFn_RoomTimer_API_Check);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_SetRoomUpdateTime", LuaFn_Room_API_SetRoomUpdateTime);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_CheckRoomUpdateTime", LuaFn_Room_API_CheckRoomUpdateTime);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_SetRoomPlayerUpdateTime", LuaFn_Room_API_SetRoomPlayerUpdateTime);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_CheckRoomPlayerUpdateTime", LuaFn_Room_API_CheckRoomPlayerUpdateTime);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_GetPlayerCount", LuaFn_Room_API_GetPlayerCount);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_GetVisitorPlayerCount", LuaFn_Room_API_GetVisitorPlayerCount);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_GetRoomState", LuaFn_Room_API_GetRoomState);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_SetRoomState", LuaFn_Room_API_SetRoomState);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_DealCode", LuaFn_Room_API_DealCode);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_GetRoomCurrPlayerID", LuaFn_Room_API_GetRoomCurrPlayerID);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_SetPlayerBet", LuaFn_Room_API_SetPlayerBet);

	//房间内游戏相关API
	m_LuaFnRegister.AddFunc("LuaFn_Room_GameInfo_API_GetPostion", LuaFn_Room_GameInfo_API_GetPostion);
	m_LuaFnRegister.AddFunc("LuaFn_Room_GameInfo_API_SetPostion", LuaFn_Room_GameInfo_API_SetPostion);
	m_LuaFnRegister.AddFunc("LuaFn_Room_GameInfo_API_GetGameState", LuaFn_Room_GameInfo_API_GetGameState);
	m_LuaFnRegister.AddFunc("LuaFn_Room_GameInfo_API_SetGameState", LuaFn_Room_GameInfo_API_SetGameState);
	m_LuaFnRegister.AddFunc("LuaFn_Room_GameInfo_API_GetCurrentSmallBlind", LuaFn_Room_GameInfo_API_GetCurrentSmallBlind);
	m_LuaFnRegister.AddFunc("LuaFn_Room_GameInfo_API_SetCurrentSmallBlind", LuaFn_Room_GameInfo_API_SetCurrentSmallBlind);
	m_LuaFnRegister.AddFunc("LuaFn_Room_GameInfo_API_GetRoundCount", LuaFn_Room_GameInfo_API_GetRoundCount);
	m_LuaFnRegister.AddFunc("LuaFn_Room_GameInfo_API_SetRoundCount", LuaFn_Room_GameInfo_API_SetRoundCount);
	m_LuaFnRegister.AddFunc("LuaFn_Room_GameInfo_API_GetTempBet", LuaFn_Room_GameInfo_API_GetTempBet);
	m_LuaFnRegister.AddFunc("LuaFn_Room_GameInfo_API_SetTempBet", LuaFn_Room_GameInfo_API_SetTempBet);
	m_LuaFnRegister.AddFunc("LuaFn_Room_GameInfo_API_GetMoney", LuaFn_Room_GameInfo_API_GetMoney);
	m_LuaFnRegister.AddFunc("LuaFn_Room_GameInfo_API_SetMoney", LuaFn_Room_GameInfo_API_SetMoney);
	m_LuaFnRegister.AddFunc("LuaFn_Room_GameInfo_API_GetCurrPlayerID", LuaFn_Room_GameInfo_API_GetCurrPlayerID);
	m_LuaFnRegister.AddFunc("LuaFn_Room_GameInfo_API_SetCurrPlayerID", LuaFn_Room_GameInfo_API_SetCurrPlayerID);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_GetRoomNextPlayerID", LuaFn_Room_API_GetRoomNextPlayerID);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_SetBenginePlayerID", LuaFn_Room_API_SetBenginePlayerID);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_GetBenginePlayerID", LuaFn_Room_API_GetBenginePlayerID);

	//发送相关API
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_SendRoomState", LuaFn_Room_API_SendRoomState);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_SendRoomSitdown", LuaFn_Room_API_SendRoomSitdown);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_SendRoomTimer", LuaFn_Room_API_SendRoomTimer);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_SendRoomPlayerTimer", LuaFn_Room_API_SendRoomPlayerTimer);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_SendRoomMoney", LuaFn_Room_API_SendRoomMoney);
	m_LuaFnRegister.AddFunc("LuaFn_Room_API_SendRoomTableCode", LuaFn_Room_API_SendRoomTableCode);
	
	for(int i = 0; i < m_LuaFnRegister.GetSize(); i++)
	{
		_FuncInfo* pFuncInfo = m_LuaFnRegister.GetFuncInfo(i);
		if(NULL != pFuncInfo)
		{
			tolua_function(m_pState, pFuncInfo->m_strFuncName.c_str(), pFuncInfo->m_FuncExecute);
		}
	}
	tolua_endmodule(m_pState);


	return true;
}

bool CLuaFn::GetLuaGlobal_Int(const char* pGlobalName, int& nGlobal)
{
	lua_getglobal(m_pState, pGlobalName);
	if(lua_isnumber(m_pState, -1) == 1)
	{
		nGlobal = (int)lua_tonumber(m_pState, -1);
		lua_pop(m_pState, -1);
		return true; 
	}
	else
	{
		return false;
	}
}

bool CLuaFn::GetLuaGlobal_String(const char* pGlobalName, string& strGlobal)
{
	lua_getglobal(m_pState, pGlobalName);
	if(lua_isstring(m_pState, -1) == 1)
	{
		char* pGlobal = (char* )lua_tostring(m_pState, -1);
		if(NULL == pGlobal)
		{
			return false;
		}
		else
		{
			strGlobal = pGlobal;
			lua_pop(m_pState, -1);
			return true; 
		}
	}
	else
	{
		return false;
	}
}

bool CLuaFn::GetLuaGlobal_ArrayCount(const char* pGlobalName, int& nCount)
{
	char szCommand[MAX_LUA_BUFF_200] = {'\0'};
	ACE_OS::sprintf(szCommand, "return #%s;", pGlobalName);
	luaL_dostring(m_pState, szCommand);
	nCount = (int)lua_tonumber(m_pState, -1);
	lua_pop(m_pState, -1);
	return true;
}

bool CLuaFn::GetLuaGlobal_ArrayIndex(const char* pGlobalName, int nIndex, string& strGlobal)
{
	char szCommand[MAX_LUA_BUFF_200] = {'\0'};
	ACE_OS::sprintf(szCommand, "return %s[%d];", pGlobalName, nIndex);
	luaL_dostring(m_pState, szCommand);
	strGlobal = (string)lua_tostring(m_pState, -1);
	lua_pop(m_pState, -1);
	return true;
}


bool CLuaFn::PushLuaData(lua_State* pState, _ParamData* pParam)
{
	if(pParam == NULL)
	{
		return false;
	}

	if(pParam->CompareType("string"))
	{
		lua_pushstring(m_pState, (char* )pParam->GetParam());
		return true;
	}

	if(pParam->CompareType("int"))
	{
		int* nData = (int* )pParam->GetParam();
		lua_pushnumber(m_pState, *nData);
		return true;
	}
	else
	{
		void* pVoid = pParam->GetParam();
		tolua_pushusertype(m_pState, pVoid, pParam->GetType());
		return true;
	}
}

bool CLuaFn::PopLuaData(lua_State* pState, _ParamData* pParam, int nIndex)
{
	if(pParam == NULL)
	{
		return false;
	}

	if(pParam->CompareType("string"))
	{
		if (lua_isstring(m_pState, nIndex) == 1)
		{
			const char* pData = (const char*)lua_tostring(m_pState, nIndex);
			pParam->SetData((void* )pData, (int)strlen(pData));
		}
		return true;
	}

	if(pParam->CompareType("int"))
	{
		if (lua_isnumber(m_pState, nIndex) == 1)
		{
			int nData = (int)lua_tonumber(m_pState, nIndex);
			pParam->SetData(&nData, sizeof(int));
		}
		return true;
	}
	else
	{
		pParam->SetData(tolua_tousertype(m_pState, nIndex, NULL), -1);
		return true;
	}
}
