--�ڷ����е��߼�
--add by freeeyes

ROOM_PLAYER_SIZE         = 5    --��������Ҳμӵ��������
ROOM_PLAYER_VISITOR_SIZE = 5    --�۲�����ҵ�������

ROOM_ERROR_NULL          = 0    --��ȷ�ķ���ֵ

--��ҽ��뷿�䣬nRoomID������ID��nPlayerID�����ID
function EnterRoom(nRoomID, nPlayerID)
	--�����������λ����������ʾ
	if LuaFn_Room_API_GetPlayerCount(nRoomID) > ROOM_PLAYER_SIZE then
		LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] Room Player is full.")
		return -1
	end
	
	--��������ڲι��������������ʾ
	if LuaFn_Room_API_GetVisitorPlayerCount(nRoomID) > ROOM_PLAYER_SIZE then
		LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] Room Player is full.")
		return -1
	end	
	
	--���뷿��
	nRet = LuaFn_Room_API_EnterRoom(nRoomID, nPlayerID, "freeeyes", 1000)
	if nRet ~= ROOM_ERROR_NULL then
		LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] EnterRoom fail.[nRet="..nRet.."]")
		return -1
	else
		--LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] EnterRoom success.")
		
		--�ⲿ��Ϊ���Դ��룬�������ݸ���
		--nCount        = LuaFn_Room_API_GetPlayerCount(nRoomID)
		--nVisitorCount = LuaFn_Room_API_GetVisitorPlayerCount(nRoomID)
		--LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] EnterRoom [Count="..nCount.."][Visit Count="..nVisitorCount.."]")		
		return 0
	end
end

--������£�nRoomID������ID��nPlayerID�����ID, nLocation������λ��
function SitRoom(nRoomID, nPlayerID, nLocation)
	--��������
	nRet = LuaFn_Room_API_PlayerSit(nRoomID, nPlayerID, nLocation)
	if nRet ~= ROOM_ERROR_NULL then
		LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] SitRoom fail.[nRet="..nRet.."]")
		return -1
	else
		--LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] SitRoom success.")
		
		--�жϷ����Ƿ���ڶ�ʱ��
		if LuaFn_RoomTimer_API_Check(nRoomID) == false then
			--���û�ж�ʱ�������չ�����һ����ʱ��
			nCount        = LuaFn_Room_API_GetPlayerCount(nRoomID)
			nVisitorCount = LuaFn_Room_API_GetVisitorPlayerCount(nRoomID)
			LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] SitRoom [Count="..nCount.."][Visit Count="..nVisitorCount.."]")
			if nCount > 1 then
				--�жϵ�ǰʱ���Ƿ���ڷ������ʱ��5��
				if LuaFn_Room_API_CheckRoomUpdateTime(nRoomID, 5) == true then
					--���һ������Ķ�ʱ��
					nRet = LuaFn_RoomTimer_API_Add(2, nRoomID, nPlayerID, 1001)
					if nRet <= 0 then
						LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] SitRoom timer fail.[nRet="..nRet.."]")
					else
						--LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] SitRoom timer success.[nsecond=5]")
					end
				else
					--���һ������Ķ�ʱ��
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

--����뿪����nRoomID������ID��nPlayerID�����ID
function ExitRoom(nRoomID, nPlayerID)
	--���Դ���
	--nCount        = LuaFn_Room_API_GetPlayerCount(nRoomID)
	--nVisitorCount = LuaFn_Room_API_GetVisitorPlayerCount(nRoomID)
	--LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] ExitRoom [Count="..nCount.."][Visit Count="..nVisitorCount.."]")
	
	--�����һ���û�
	--nPlayerID1 = LuaFn_Room_API_GetRoomNextPlayerID(nRoomID)
	--nPlayerID2 = LuaFn_Room_API_GetRoomNextPlayerID(nRoomID)
	--LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] ExitRoom [nPlayerID1="..nPlayerID1.."][nPlayerID2="..nPlayerID2.."]")
	
	nRet = LuaFn_Room_API_Exit(nRoomID, nPlayerID)
	if nRet ~= ROOM_ERROR_NULL then
		LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] ExitRoom fail.[nRet="..nRet.."]")
		return -1
	else
		--LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] ExitRoom success.")
		
		--���Դ���
		--nCount        = LuaFn_Room_API_GetPlayerCount(nRoomID)
		--nVisitorCount = LuaFn_Room_API_GetVisitorPlayerCount(nRoomID)
		--LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] ExitRoom [Count="..nCount.."][Visit Count="..nVisitorCount.."]")
		return 0
	end
end

--�����߳���ҹ���nRoomID������ID��nPlayerID�����ID��nOperationIDc������ID
function OutRoom(nRoomID, nPlayerID, nOperationID)

end

--��ұ任���䣬nSrcRoomID��ԭʼ����ID��nDstRoomID��Ŀ�귿��ID��nPlayerID�����ID
function ChangeRoom(nSrcRoomID, nDstRoomID, nPlayerID)

end

--�����¼������ʱ���µ��á�nEventID���¼�ID��nPlayerID�������ID�����û����Ҵ�������-1
function Update(nRoomID, nPlayerID, nEventID, nData)
	--�����ע
	if nEventID == 3001 then
		nRet = LuaFn_Room_API_SetPlayerBet(nRoomID, nPlayerID, nData)
		if nRet ~= ROOM_ERROR_NULL then
			LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] Update SetPlayerBet.[nRet="..nRet.."]")
			return 0
		else
			--�ƶ�����һ�����
			nNextPlayer = LuaFn_Room_API_GetRoomNextPlayerID(nRoomID)
			if nNextPlayer > 0 then
				--���㵱ǰ�ִ��ǲ������һ��
				if LuaFn_Room_GameInfo_API_GetRoundCount(nRoomID) >= 4 then
					LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] Update LuaFn_Room_GameInfo_API_GetRoundCount.[RoundCount=4]")
					--����������ƾֽ���
				else
			
					--����һ���������װ�϶�ʱ��
					LuaFn_PlayerTimer_API_Add(5, nRoomID, nNextPlayer, 2001)
				end
			else
				LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] Update GetRoomNextPlayerID.[nRet="..nRet.."]")
			end
		end
	end
	
	return 0
end

--�����䣬ȫ���˳�
function ClearRoom()

end

--��ʼ������
function InitRoom()

end

--���Player���ϵĶ�ʱ������
function PlayerTimer(nRoomID, nPlayerID, nEventID)
	if nEventID == 2001 then
		--�������û����ע
		LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] PlayerTimer Begin")
		
		--�����ﴦ������ƶ��Ĺ���
	end
end

--����Ķ�ʱ������
function RoomTimer(nRoomID, nPlayerID, nEventID)
	LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] RoomTimer Begin")
	if nEventID == 1001 then
		--�����1001�¼�����ô���ǵ�ʱ�����¼�(�����C++������������˷��ƣ��´�äע��Сäע�Լ���Ƿ���״̬������)
		nRet = LuaFn_Room_API_DealCode(nRoomID, 1000, -1, -1)
		if nRet ~= ROOM_ERROR_NULL then
			LuaFn_Print("RoomID["..nRoomID.."],nPlayerID["..nPlayerID.."] RoomTimer Begin DealCode.[nRet="..nRet.."]")
			return 0
		else
			nCurrPlayerID = LuaFn_Room_API_GetRoomCurrPlayerID(nRoomID)
			
			--���ÿ�ʼ�����ID���Դ�Ϊ�ִμ�������ݣ����GetRoomNextPlayerID���������ң����ִμ���+1
			LuaFn_Room_API_SetBenginePlayerID(nCurrPlayerID)
			
			--����һ�����Ƶ������Ӷ�ʱ��
			LuaFn_PlayerTimer_API_Add(5, nRoomID, nPlayerID, 2001)
		end
	end
end

