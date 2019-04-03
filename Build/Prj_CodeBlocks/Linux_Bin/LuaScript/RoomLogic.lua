--在房间中的逻辑
--add by freeeyes

ROOM_PLAYER_SIZE         = 5    --房间内玩家参加的最大人数
ROOM_PLAYER_VISITOR_SIZE = 5    --观察者玩家的最大个数

ROOM_ERROR_NULL          = 0    --正确的返回值

--玩家进入房间，nRoomID：房间ID，nPlayerID：玩家ID
function EnterRoom(nRoomID, nPlayerID)
	--如果房间内座位已满，则提示
	if LuaFn_Room_API_GetPlayerCount(nRoomID) > ROOM_PLAYER_SIZE then
		LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] Room Player is full.")
		return -1
	end
	
	--如果房间内参观玩家已满，则提示
	if LuaFn_Room_API_GetVisitorPlayerCount(nRoomID) > ROOM_PLAYER_SIZE then
		LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] Room Player is full.")
		return -1
	end	
	
	--进入房间
	nRet = LuaFn_Room_API_EnterRoom(nRoomID, nPlayerID, "freeeyes", 1000)
	if nRet ~= ROOM_ERROR_NULL then
		LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] EnterRoom fail.[nRet="..nRet.."]")
		return -1
	else
		--LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] EnterRoom success.")
		
		--这部分为测试代码，用于数据跟踪
		--nCount        = LuaFn_Room_API_GetPlayerCount(nRoomID)
		--nVisitorCount = LuaFn_Room_API_GetVisitorPlayerCount(nRoomID)
		--LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] EnterRoom [Count="..nCount.."][Visit Count="..nVisitorCount.."]")		
		return 0
	end
end

--玩家坐下，nRoomID：房间ID，nPlayerID：玩家ID, nLocation是所在位置
function SitRoom(nRoomID, nPlayerID, nLocation)
	--尝试坐下
	nRet = LuaFn_Room_API_PlayerSit(nRoomID, nPlayerID, nLocation)
	if nRet ~= ROOM_ERROR_NULL then
		LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] SitRoom fail.[nRet="..nRet.."]")
		return -1
	else
		--LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] SitRoom success.")
		
		--判断房间是否存在定时器
		if LuaFn_RoomTimer_API_Check(nRoomID) == false then
			--如果没有定时器，则按照规则做一个定时器
			nCount        = LuaFn_Room_API_GetPlayerCount(nRoomID)
			nVisitorCount = LuaFn_Room_API_GetVisitorPlayerCount(nRoomID)
			LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] SitRoom [Count="..nCount.."][Visit Count="..nVisitorCount.."]")
			if nCount > 1 then
				--判断当前时间是否大于房间更新时间5秒
				if LuaFn_Room_API_CheckRoomUpdateTime(nRoomID, 5) == true then
					--添加一个房间的定时器
					nRet = LuaFn_RoomTimer_API_Add(2, nRoomID, nPlayerID, 1001)
					if nRet <= 0 then
						LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] SitRoom timer fail.[nRet="..nRet.."]")
					else
						--LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] SitRoom timer success.[nsecond=5]")
					end
				else
					--添加一个房间的定时器
					nRet = LuaFn_RoomTimer_API_Add(5, nRoomID, nPlayerID, 1001)
					if nRet <= 0 then
						LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] SitRoom timer fail.[nRet="..nRet.."]")
					else
						--LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] SitRoom timer success.[nsecond=2]")
					end				
				end
			end
		end
	end	
	return 0
end

--玩家离开房间nRoomID：房间ID，nPlayerID：玩家ID
function ExitRoom(nRoomID, nPlayerID)
	--调试代码
	--nCount        = LuaFn_Room_API_GetPlayerCount(nRoomID)
	--nVisitorCount = LuaFn_Room_API_GetVisitorPlayerCount(nRoomID)
	--LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] ExitRoom [Count="..nCount.."][Visit Count="..nVisitorCount.."]")
	
	--获得下一个用户
	--nPlayerID1 = LuaFn_Room_API_GetRoomNextPlayerID(nRoomID)
	--nPlayerID2 = LuaFn_Room_API_GetRoomNextPlayerID(nRoomID)
	--LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] ExitRoom [nPlayerID1="..nPlayerID1.."][nPlayerID2="..nPlayerID2.."]")
	
	nRet = LuaFn_Room_API_Exit(nRoomID, nPlayerID)
	if nRet ~= ROOM_ERROR_NULL then
		LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] ExitRoom fail.[nRet="..nRet.."]")
		return -1
	else
		--LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] ExitRoom success.")
		
		--调试代码
		--nCount        = LuaFn_Room_API_GetPlayerCount(nRoomID)
		--nVisitorCount = LuaFn_Room_API_GetVisitorPlayerCount(nRoomID)
		--LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] ExitRoom [Count="..nCount.."][Visit Count="..nVisitorCount.."]")
		return 0
	end
end

--房间踢出玩家规则nRoomID：房间ID，nPlayerID：玩家ID，nOperationIDc操作者ID
function OutRoom(nRoomID, nPlayerID, nOperationID)

end

--玩家变换房间，nSrcRoomID：原始房间ID，nDstRoomID：目标房间ID，nPlayerID：玩家ID
function ChangeRoom(nSrcRoomID, nDstRoomID, nPlayerID)

end

--房间事件到达，定时更新调用。nEventID，事件ID，nPlayerID触发玩家ID，如果没有玩家触发则是-1
function Update(nRoomID, nPlayerID, nEventID, nData)
	--玩家下注
	if nEventID == 3001 then
		nRet = LuaFn_Room_API_SetPlayerBet(nRoomID, nPlayerID, nData)
		if nRet ~= ROOM_ERROR_NULL then
			LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] Update SetPlayerBet.[nRet="..nRet.."]")
			return 0
		else
			--移动到下一个玩家
			nNextPlayer = LuaFn_Room_API_GetRoomNextPlayerID(nRoomID)
			if nNextPlayer > 0 then
				--计算当前轮次是不是最后一轮
				if LuaFn_Room_GameInfo_API_GetRoundCount(nRoomID) >= 4 then
					LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] Update LuaFn_Room_GameInfo_API_GetRoundCount.[RoundCount=4]")
					--在这里进行牌局结算
				else
			
					--给下一个玩家身上装上定时器
					LuaFn_PlayerTimer_API_Add(5, nRoomID, nNextPlayer, 2001)
				end
			else
				LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] Update GetRoomNextPlayerID.[nRet="..nRet.."]")
			end
		end
	end
	
	return 0
end

--清理房间，全部退出
function ClearRoom()

end

--初始化房间
function InitRoom()

end

--玩家Player身上的定时器到达
function PlayerTimer(nRoomID, nPlayerID, nEventID)
	if nEventID == 2001 then
		--到期玩家没有下注
		LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] PlayerTimer Begin")
		
		--在这里处理玩家移动的规则
	end
end

--房间的定时器到达
function RoomTimer(nRoomID, nPlayerID, nEventID)
	LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] RoomTimer Begin")
	if nEventID == 1001 then
		--如果是1001事件，那么就是到时发牌事件(在这个C++函数里面完成了发牌，下大盲注，小盲注以及标记房间状态的任务)
		nRet = LuaFn_Room_API_DealCode(nRoomID, 1000, -1, -1)
		if nRet ~= ROOM_ERROR_NULL then
			LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] RoomTimer Begin DealCode.[nRet="..nRet.."]")
			return 0
		else
			nCurrPlayerID = LuaFn_Room_API_GetRoomCurrPlayerID(nRoomID)
			
			--设置开始的玩家ID，以此为轮次计算的依据，如果GetRoomNextPlayerID到了这个玩家，则轮次计算+1
			LuaFn_Room_API_SetBenginePlayerID(nCurrPlayerID)
			
			--对下一个出牌的玩家添加定时器
			LuaFn_PlayerTimer_API_Add(5, nRoomID, nPlayerID, 2001)
		end
	end
end

