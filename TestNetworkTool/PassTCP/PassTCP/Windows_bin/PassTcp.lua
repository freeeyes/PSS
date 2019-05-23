--这是一个标准的Lua PassTcp脚本
--提供发送事件的封装和接收到数据事件的封装
--你可以在这里组织你的发送数据和验证接收数据
--这个接口这样设计，主要是考虑到多线程操作，尽量减少不必要的线程锁
--所以这里的数据块不要在脚本里声明出来，外层不管释放
--目前最大支持100K的数据块大小
--add by freeeyes

--要发送数据的事件，由PassTCP调用，这里实现你的发送数据封包
--入口参数，nThreadID是当前线程ID，szData是信息内容(char* 数组)，nLen是缓冲区最大长度，nSendIndex是当前连接的发送次数
function PassTcp_CreateSendData(szData, nLen, nThreadID, nSendIndex)
    --创建发送数据
	--szData为char* 是你要填充的缓冲块。
	--nLen是当前缓冲块最大长度
	--如果nNextIndex返回-1，则说明赋值错误,否则返回下一次的起始的位置
	
	--添加4字节包头
	nPacketSize = 10
	nStartIndex  = 0
	nNextIndex  = 0
	nNextIndex = Lua_Tcp_Buffer_In_Int32(szData, nPacketSize, nStartIndex, nLen)
	
	--添加2字节的命令字
	nCommandID = 4096
	nNextIndex = Lua_Tcp_Buffer_In_Int16(szData, nCommandID, nNextIndex, nLen)
	
	--添加数据块
	szBuff = '12345678'
	nNextIndex = Lua_Tcp_Buffer_In_Block(szData, szBuff, 8, nNextIndex, nLen)
	
	--这里必须返回填充数据块的长度，让PassTCP知道发送数据总长度
	return nNextIndex
end

--接收到的数据事件
--入口参数，nThreadID是当前线程ID，szData是接收到的内容(char* 数组)，nLen是当前接收的长度，nSendIndex是当前连接的发送次数
function PassTcp_GetRecvData(szData, nLen, nThreadID)
    --得到接收数据
	--szData是当前收到的数据块
	--nLen是收到的数据库块长度
	--函数的返回，是一个int类型
	--目前这么定义，   0：接收数据包成功，验证合法。成功的数据包+1
	--                 1：接收的包没有结束，还需继续接收
	--                 2：接收数据包验证失败，即是错误的数据包+1
	
	--判断接收数据包的长度
	nCurrIndex = 0;
	nPacketLength = Lua_Tcp_Buffer_Out_Int32(szData, nCurrIndex, nLen, nSendIndex);
	if nPacketLength ~= 10 then
		return 2;
	end
	
	--判断接收的返回命令ID是否正确
	nCurrIndex = 4
	nCommandID = Lua_Tcp_Buffer_Out_Int16(szData, nCurrIndex, nLen);
	if nCommandID ~= 4096 then
		return 2;
	end
	
	--包验证成功
	return 0;
end


