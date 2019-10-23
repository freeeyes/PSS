# PSS插件允许使用核心API命令说明
## 1.信令增删改管理（IMessageManager）
### 说明:  
> 这个接口提供插件可以任意的添加，删除，修改，对指定信令的关联。  
> 比如你在运行时需要临时添加一个信令(0x1050)在自己插件处理，可以调用这个借口实现框架的绑定。  

###在插件中使用方法:
m_pServerObject->GetMessageManager()  

###API列表：  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th width="20%">接口名称</th>
			<th width="20%">说明</th>
            <th width="60%" colspan="3">接口参数</th>
        </tr>
        <tr>
            <th rowspan="6">AddClientCommand</th>
            <th rowspan="6">添加指定的信令</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>u2CommandID</th></th><th>添加信令ID</th></tr>
		<tr><th>CClientCommand*</th><th>pClientCommand</th><th>对应信令处理类指针</th></tr>
		<tr><th>const char*</th><th>pModuleName</th><th>当前模块名称</th></tr>
		<tr><th>_ClientIPInfo</th><th>objListenInfo</th><th>信令绑定给指定的监听端口</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否添加成功</th></tr>
        <tr>
            <th rowspan="5">AddClientCommand</th>
            <th rowspan="5">添加指定的信令</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>u2CommandID</th></th><th>添加信令ID</th></tr>
		<tr><th>CClientCommand*</th><th>pClientCommand</th><th>对应信令处理类指针</th></tr>
		<tr><th>const char*</th><th>pModuleName</th><th>当前模块名称</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否添加成功</th></tr>
        <tr>
            <th rowspan="4">DelClientCommand</th>
            <th rowspan="4">删除指定的信令</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>u2CommandID</th></th><th>删除信令ID</th></tr>
		<tr><th>CClientCommand*</th><th>pClientCommand</th><th>对应信令处理类指针</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否删除成功</th></tr>
</table>  

## 2.日志输出管理（ILogManager）
### 说明:  
> 这个接口提供插件日志的写入。 包括对文本和二进制数据的支持。  
> 也可以用这个接口发送邮件给指定的邮件服务器。   

###在插件中使用方法:
m_pServerObject->GetLogManager()  

###API列表：  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th width="20%">接口名称</th>
			<th width="20%">说明</th>
            <th width="60%" colspan="3">接口参数</th>
        </tr>
        <tr>
            <th rowspan="5">WriteLog</th>
            <th rowspan="5">记录文本日志</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>int</th><th>nLogType</th></th><th>日志类型,这里你可以使用自己的类型，也可以使用框架提供的日志支持类型，框架支持的日志类型定义在logger.xml中</th></tr>
		<tr><th>const char*</th><th>fmt</th><th>日志内容</th></tr>
		<tr><th>---</th><th>...</th><th>日志内容参数</th></tr>
		<tr><th>int</th><th>返回值</th><th>是否执行成功</th></tr>
        <tr>
            <th rowspan="5">WriteLogBinary</th>
            <th rowspan="5">记录二进制日志</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>int</th><th>nLogType</th></th><th>日志类型,这里你可以使用自己的类型，也可以使用框架提供的日志支持类型，框架支持的日志类型定义在logger.xml中</th></tr>
		<tr><th>const char*</th><th>pClientCommand</th><th>二进制数据指针</th></tr>
		<tr><th>int</th><th>nLen</th><th>二进制数据长度</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
        <tr>
            <th rowspan="6">WriteToMail</th>
            <th rowspan="6">发送邮件</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>int</th><th>nLogType</th></th><th>日志类型,这里你可以使用自己的类型，也可以使用框架提供的日志支持类型，框架支持的日志类型定义在logger.xml中</th></tr>
		<tr><th>uint32</th><th>u4MailID</th><th>对应邮件配置参数ID,这个ID配置在alert.xml中的Mail信息中</th></tr>
		<tr><th>const char*</th><th>pTitle</th><th>邮件标题</th></tr>
		<tr><th>const char*</th><th>fmt</th><th>邮件内容</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否添加成功</th></tr>
</table>  

## 3.客户端连接管理（IConnectManager）
### 说明:  
> 这个接口管理所有连接上PSS的客户端，并支持发送数据，查看状态等操作。  

###在插件中使用方法:
m_pServerObject->GetConnectManager()  

###API列表：  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th width="20%">接口名称</th>
			<th width="20%">说明</th>
            <th width="60%" colspan="3">接口参数</th>
        </tr>
        <tr>
            <th rowspan="9">PostMessage</th>
            <th rowspan="9">发送数据给指定的客户端</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint32</th><th>u4ConnectID</th></th><th>客户端的连接ID</th></tr>
		<tr><th>IBuffPacket*&</th><th>pBuffPacket</th><th>要发送的内容</th></tr>
		<tr><th>uint8</th><th>u1SendType</th><th>SENDMESSAGE_NOMAL默认发送数据包模式(走PacketPrase端口),  SENDMESSAGE_JAMPNOMAL发送数据包模式(不走PacketPrase端口)</th></tr>
		<tr><th>uint16</th><th>u2CommandID</th><th>要发送的信令ID</th></tr>
		<tr><th>uint8</th><th>u1SendState</th><th>包括以下几种状态PACKET_SEND_IMMEDIATLY(立刻发送),PACKET_SEND_CACHE(数据包发送机制)和PACKET_SEND_FIN_CLOSE(立刻发送，发送完成后关闭连接)</th></tr>
		<tr><th>bool</th><th>blDlete</th><th>是否由框架删除数据包内存块，true是框架删除，false由插件自己管理内存块的释放</th></tr>
		<tr><th>int</th><th>nMessageID</th><th>这个参数用于数据回执，如果ID > 0，则如果发送成功，会回调CLINET_LINK_SENDOK，插件可以关注这个事件获得结果，发送失败会调用CLINET_LINK_SENDERROR</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
        </tr>
        <tr>
            <th rowspan="10">PostMessage</th>
            <th rowspan="10">发送数据给指定的客户端</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint32>/th><th>u4ConnectID</th></th><th>客户端的连接ID列表</th></tr>
		<tr><th>char*&</th><th>pData</th><th>要发送的数据内容指针</th></tr>
		<tr><th>uint32</th><th>nDataLen</th><th>要发送的数据内容长度</th></tr>
		<tr><th>uint8</th><th>u1SendType</th><th>SENDMESSAGE_NOMAL默认发送数据包模式(走PacketPrase端口),  SENDMESSAGE_JAMPNOMAL发送数据包模式(不走PacketPrase端口)</th></tr>
		<tr><th>uint16</th><th>u2CommandID</th><th>要发送的信令ID</th></tr>
		<tr><th>uint8</th><th>u1SendState</th><th>包括以下几种状态PACKET_SEND_IMMEDIATLY(立刻发送),PACKET_SEND_CACHE(数据包发送机制)和PACKET_SEND_FIN_CLOSE(立刻发送，发送完成后关闭连接)</th></tr>
		<tr><th>bool</th><th>blDlete</th><th>是否由框架删除数据包内存块，true是框架删除，false由插件自己管理内存块的释放</th></tr>
		<tr><th>int</th><th>nMessageID</th><th>这个参数用于数据回执，如果ID > 0，则如果发送成功，会回调CLINET_LINK_SENDOK，插件可以关注这个事件获得结果，发送失败会调用CLINET_LINK_SENDERROR</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
        <tr>
            <th rowspan="9">PostMessage</th>
            <th rowspan="9">发送数据给指定的客户端</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>vector</th><th>vecConnectID</th></th><th>客户端的连接ID列表</th></tr>
		<tr><th>IBuffPacket*&</th><th>pBuffPacket</th><th>要发送的内容</th></tr>
		<tr><th>uint8</th><th>u1SendType</th><th>SENDMESSAGE_NOMAL默认发送数据包模式(走PacketPrase端口),  SENDMESSAGE_JAMPNOMAL发送数据包模式(不走PacketPrase端口)</th></tr>
		<tr><th>uint16</th><th>u2CommandID</th><th>要发送的信令ID</th></tr>
		<tr><th>uint8</th><th>u1SendState</th><th>包括以下几种状态PACKET_SEND_IMMEDIATLY(立刻发送),PACKET_SEND_CACHE(数据包发送机制)和PACKET_SEND_FIN_CLOSE(立刻发送，发送完成后关闭连接)</th></tr>
		<tr><th>bool</th><th>blDlete</th><th>是否由框架删除数据包内存块，true是框架删除，false由插件自己管理内存块的释放</th></tr>
		<tr><th>int</th><th>nMessageID</th><th>这个参数用于数据回执，如果ID > 0，则如果发送成功，会回调CLINET_LINK_SENDOK，插件可以关注这个事件获得结果，发送失败会调用CLINET_LINK_SENDERROR</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
        </tr>
        <tr>
            <th rowspan="10">PostMessage</th>
            <th rowspan="10">发送数据给指定的客户端</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>vector</th><th>vecConnectID</th></th><th>客户端的连接ID列表</th></tr>
		<tr><th>char*&</th><th>pData</th><th>要发送的数据内容指针</th></tr>
		<tr><th>uint32</th><th>nDataLen</th><th>要发送的数据内容长度</th></tr>
		<tr><th>uint8</th><th>u1SendType</th><th>SENDMESSAGE_NOMAL默认发送数据包模式(走PacketPrase端口),  SENDMESSAGE_JAMPNOMAL发送数据包模式(不走PacketPrase端口)</th></tr>
		<tr><th>uint16</th><th>u2CommandID</th><th>要发送的信令ID</th></tr>
		<tr><th>uint8</th><th>u1SendState</th><th>包括以下几种状态PACKET_SEND_IMMEDIATLY(立刻发送),PACKET_SEND_CACHE(数据包发送机制)和PACKET_SEND_FIN_CLOSE(立刻发送，发送完成后关闭连接)</th></tr>
		<tr><th>bool</th><th>blDlete</th><th>是否由框架删除数据包内存块，true是框架删除，false由插件自己管理内存块的释放</th></tr>
		<tr><th>int</th><th>nMessageID</th><th>这个参数用于数据回执，如果ID > 0，则如果发送成功，会回调CLINET_LINK_SENDOK，插件可以关注这个事件获得结果，发送失败会调用CLINET_LINK_SENDERROR</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
        <tr>
            <th rowspan="8">PostMessage</th>
            <th rowspan="8">群发数据给所有客户端</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>IBuffPacket*&</th><th>pBuffPacket</th><th>要发送的内容</th></tr>
		<tr><th>uint8</th><th>u1SendType</th><th>SENDMESSAGE_NOMAL默认发送数据包模式(走PacketPrase端口),  SENDMESSAGE_JAMPNOMAL发送数据包模式(不走PacketPrase端口)</th></tr>
		<tr><th>uint16</th><th>u2CommandID</th><th>要发送的信令ID</th></tr>
		<tr><th>uint8</th><th>u1SendState</th><th>包括以下几种状态PACKET_SEND_IMMEDIATLY(立刻发送),PACKET_SEND_CACHE(数据包发送机制)和PACKET_SEND_FIN_CLOSE(立刻发送，发送完成后关闭连接)</th></tr>
		<tr><th>bool</th><th>blDlete</th><th>是否由框架删除数据包内存块，true是框架删除，false由插件自己管理内存块的释放</th></tr>
		<tr><th>int</th><th>nMessageID</th><th>这个参数用于数据回执，如果ID > 0，则如果发送成功，会回调CLINET_LINK_SENDOK，插件可以关注这个事件获得结果，发送失败会调用CLINET_LINK_SENDERROR</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
        </tr>
        <tr>
            <th rowspan="9">PostMessage</th>
            <th rowspan="9">群发数据给所有客户端</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>char*&</th><th>pData</th><th>要发送的数据内容指针</th></tr>
		<tr><th>uint32</th><th>nDataLen</th><th>要发送的数据内容长度</th></tr>
		<tr><th>uint8</th><th>u1SendType</th><th>SENDMESSAGE_NOMAL默认发送数据包模式(走PacketPrase端口),  SENDMESSAGE_JAMPNOMAL发送数据包模式(不走PacketPrase端口)</th></tr>
		<tr><th>uint16</th><th>u2CommandID</th><th>要发送的信令ID</th></tr>
		<tr><th>uint8</th><th>u1SendState</th><th>包括以下几种状态PACKET_SEND_IMMEDIATLY(立刻发送),PACKET_SEND_CACHE(数据包发送机制)和PACKET_SEND_FIN_CLOSE(立刻发送，发送完成后关闭连接)</th></tr>
		<tr><th>bool</th><th>blDlete</th><th>是否由框架删除数据包内存块，true是框架删除，false由插件自己管理内存块的释放</th></tr>
		<tr><th>int</th><th>nMessageID</th><th>这个参数用于数据回执，如果ID > 0，则如果发送成功，会回调CLINET_LINK_SENDOK，插件可以关注这个事件获得结果，发送失败会调用CLINET_LINK_SENDERROR</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
        <tr>
            <th rowspan="3">CloseConnect</th>
            <th rowspan="3">断开指定的客户端连接</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint32</th><th>u4ConnectID</th><th>客户端连接ID</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
        <tr>
            <th rowspan="3">GetClientIPInfo</th>
            <th rowspan="3">得到指定客户端连接客户IP信息</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint32</th><th>u4ConnectID</th><th>客户端连接ID</th></tr>
		<tr><th>_ClientIPInfo</th><th>返回值</th><th>客户端IP信息</th></tr>
        <tr>
            <th rowspan="3">GetLocalIPInfo</th>
            <th rowspan="3">得到指定客户端连接服务器IP信息</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint32</th><th>u4ConnectID</th><th>客户端连接ID</th></tr>
		<tr><th>_ClientIPInfo</th><th>返回值</th><th>服务器IP信息</th></tr>
        <tr>
            <th rowspan="2">GetCount</th>
            <th rowspan="2">得到当前客户端连接数量</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>int</th><th>返回值</th><th>当前连接数量</th></tr>
        <tr>
            <th rowspan="4">SetConnectName</th>
            <th rowspan="4">给一个特定的客户端连接起名</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint32</th><th>u4ConnectID</th><th>客户端连接ID</th></tr>
		<tr><th>const char*</th><th>pName</th><th>客户端连接ID名字</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
        <tr>
            <th rowspan="4">SetIsLog</th>
            <th rowspan="4">给一个客户端连接打开日志跟踪</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint32</th><th>u4ConnectID</th><th>客户端连接ID</th></tr>
		<tr><th>bool</th><th>blIsLog</th><th>是否打开日志跟踪</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
        <tr>
            <th rowspan="4">GetClientNameInfo</th>
            <th rowspan="4">根据一个客户端名字获得对应的客户端连接信息</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint32</th><th>u4ConnectID</th><th>客户端连接ID</th></tr>
		<tr><th>vecClientNameInfo</th><th>objClientNameInfo</th><th>对应名称的客户端连接信息列表</th></tr>
		<tr><th>void</th><th>返回值</th><th>无</th></tr>
        <tr>
            <th rowspan="3">GetClientNameInfo</th>
            <th rowspan="3">得到指定的客户端连接状态,false为链接不存在，true为链接存在</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint32</th><th>u4ConnectID</th><th>客户端连接ID</th></tr>
		<tr><th>EM_Client_Connect_status</th><th>返回值</th><th>true为存在，false为不存在</th></tr>
</table>  

## 4.Packet对象管理类（IPacketManager）
### 说明:  
> 这个接口提供CBuffPacket对象的管理。  

###在插件中使用方法:
m_pServerObject->GetPacketManager()  

###API列表：  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th width="20%">接口名称</th>
			<th width="20%">说明</th>
            <th width="60%" colspan="3">接口参数</th>
        </tr>
        <tr>
            <th rowspan="2">GetBuffPacketUsedCount</th>
            <th rowspan="2">得到当前已使用的IBuffPacket数量</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint32</th><th>返回值</th><th>当前已使用的个数</th></tr>
        <tr>
            <th rowspan="2">GetBuffPacketFreeCount</th>
            <th rowspan="2">得到当前未使用的IBuffPacket数量</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint32</th><th>返回值</th><th>当前未使用的个数</th></tr>
        <tr>
            <th rowspan="2">OutputCreateInfo</th>
            <th rowspan="2">输出当前IBuffPecket使用情况到日志中</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>void</th><th>返回值</th><th>无</th></tr>
        <tr>
            <th rowspan="3">SetCreateFlag</th>
            <th rowspan="3">是否记录每个CBuffPacket从哪里申请出来的</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>bool</th><th>blTagCreateInfo</th><th>是否记录申请位置</th></tr>
		<tr><th>void</th><th>返回值</th><th>无</th></tr>
        <tr>
            <th rowspan="4">Create</th>
            <th rowspan="4">获得一个空闲的IBuffPacke对象</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>const char*</th><th>pFileName</th><th>申请对象代码文件名称</th></tr>
		<tr><th>uint32</th><th>u4Line</th><th>申请对象代码行号</th></tr>
		<tr><th>IBuffPacket*</th><th>返回值</th><th>IBuffPacket对象指针</th></tr>
        <tr>
            <th rowspan="3">Delete</th>
            <th rowspan="3">回收一个IBuffPacket对象</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>IBuffPacket*</th><th>pFileName</th><th>IBuffPacket对象指针</th></tr>
		<tr><th>bool*</th><th>返回值</th><th>是否回收成功</th></tr>
        <tr>
            <th rowspan="2">GetCreateInfoCount</th>
            <th rowspan="2">获得当前申请IBuffPacket的文件信息个数</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint32</th><th>返回值</th><th>获得当前申请IBuffPacket的文件信息个数</th></tr>
        <tr>
            <th rowspan="5">GetCreateInfoList</th>
            <th rowspan="5">获得当前申请IBuffPacket的文件信息</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint32</th><th>u4Index</th><th>当前Index，对应GetCreateInfoCount接口的个数</th></tr>
		<tr><th>_Object_Create_Info</th><th>objCreateInfo</th><th>申请对象位置信息</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否获取成功</th></tr>
</table>  

## 5.服务期间对象管理类（IClientManager）
### 说明:  
> 负责管理服务器间通讯的数据管理

###在插件中使用方法:
m_pServerObject->GetClientManager()  

###API列表：  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th width="20%">接口名称</th>
			<th width="20%">说明</th>
            <th width="60%" colspan="3">接口参数</th>
        </tr>
        <tr>
            <th rowspan="7">Connect</th>
            <th rowspan="7">建立一个连接其他服务器的TCP连接</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>int</th><th>nServerID</th><th>当前连接ID</th></tr>
		<tr><th>const char*</th><th>pIP</th><th>目标服务器的IP</th></tr>
		<tr><th>int</th><th>nPort</th><th>目标服务器的端口</th></tr>
		<tr><th>uint8</th><th>u1IPType</th><th>标记是IPv4还是IPV6</th></tr>
		<tr><th>IClientMessage*</th><th>pClientMessage</th><th>处理接收服务器连接的返回数据类对象指针</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
        <tr>
            <th rowspan="10">Connect</th>
            <th rowspan="10">建立一个连接其他服务器的TCP连接</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>int</th><th>nServerID</th><th>当前连接ID</th></tr>
		<tr><th>const char*</th><th>pIP</th><th>目标服务器的IP</th></tr>
		<tr><th>int</th><th>nPort</th><th>目标服务器的端口</th></tr>
		<tr><th>uint8</th><th>u1IPType</th><th>标记是IPv4还是IPV6</th></tr>
		<tr><th>const char*</th><th>pIP</th><th>本地服务器的IP</th></tr>
		<tr><th>int</th><th>nPort</th><th>本地服务器的端口</th></tr>
		<tr><th>uint8</th><th>u1IPType</th><th>标记是IPv4还是IPV6</th></tr>
		<tr><th>IClientMessage*</th><th>pClientMessage</th><th>处理接收服务器连接的返回数据类对象指针</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
        <tr>
            <th rowspan="8">ConnectUDP</th>
            <th rowspan="8">建立一个连接其他服务器的TCP连接</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>int</th><th>nServerID</th><th>当前连接ID</th></tr>
		<tr><th>const char*</th><th>pIP</th><th>目标服务器的IP</th></tr>
		<tr><th>int</th><th>nPort</th><th>目标服务器的端口</th></tr>
		<tr><th>uint8</th><th>u1IPType</th><th>标记是IPv4还是IPV6</th></tr>
		<tr><th>const char*</th><th>pIP</th><th>UDP服务器连接状态,UDP_SINGLE UDP单独连接,UDP_BROADCAST UDP广播</th></tr>
		<tr><th>IClientMessage*</th><th>pClientMessage</th><th>处理接收服务器连接的返回数据类对象指针</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
        <tr>
            <th rowspan="3">Close</th>
            <th rowspan="3">关闭指定的TCP连接</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>int</th><th>nServerID</th><th>连接ID</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
        <tr>
            <th rowspan="3">CloseUDP</th>
            <th rowspan="3">关闭指定的UDP连接</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>int</th><th>nServerID</th><th>连接ID</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
        <tr>
            <th rowspan="6">SendData</th>
            <th rowspan="6">发送一个数据给TCP对端服务器</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>int</th><th>nServerID</th><th>连接ID</th></tr>
		<tr><th>const char*</th><th>pData</th><th>发送数据块指针</th></tr>
		<tr><th>int</th><th>nSize</th><th>发送数据长度</th></tr>
		<tr><th>bool</th><th>blIsDelete</th><th>是否框架回收，true 框架回收，false插件自己回收</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
        <tr>
            <th rowspan="8">SendDataUDP</th>
            <th rowspan="8">发送一个数据给UDP对端服务器</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>int</th><th>nServerID</th><th>连接ID</th></tr>
		<tr><th>const char*</th><th>pIP</th><th>UDP远端的IP</th></tr>
		<tr><th>int</th><th>nPort</th><th>UDP远端的端口</th></tr>
		<tr><th>const char*</th><th>pData</th><th>发送数据块指针</th></tr>
		<tr><th>int</th><th>nSize</th><th>发送数据长度</th></tr>
		<tr><th>bool</th><th>blIsDelete</th><th>是否框架回收，true 框架回收，false插件自己回收</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
        <tr>
            <th rowspan="3">GetConnectState</th>
            <th rowspan="3">得到指定TCP连接的状态</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>int</th><th>nServerID</th><th>连接ID</th></tr>
		<tr><th>EM_Server_Connect_State</th><th>返回值</th><th>返回当前连接状态</th></tr>
        <tr>
            <th rowspan="3">GetClientMessage</th>
            <th rowspan="3">得到指定连接ID的接收类对象指针</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>int</th><th>nServerID</th><th>连接ID</th></tr>
		<tr><th>IClientMessage*</th><th>返回值</th><th>返回当前连接状态</th></tr>
        <tr>
            <th rowspan="3">GetServerIPInfo</th>
            <th rowspan="3">得到指定的ID TCP连接信息</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>int</th><th>nServerID</th><th>连接ID</th></tr>
		<tr><th>_ClientIPInfo&</th><th>objServerIPInfo</th><th>返回的IP连接信息</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否找到</th></tr>
</table>  

## 6.客户端UDP连接管理（GetUDPConnectManager）
### 说明:  
> 负责管理向指定的UDP客户端端口发送数据

###在插件中使用方法:
m_pServerObject->GetClientManager()  

###API列表：  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th width="20%">接口名称</th>
			<th width="20%">说明</th>
            <th width="60%" colspan="3">接口参数</th>
        </tr>
        <tr>
            <th rowspan="10">SendMessage</th>
            <th rowspan="10">发送一个UDP数据信息</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>int</th><th>nConnectID</th><th>当前连接ID</th></tr>
		<tr><th>char*&</th><th>pMessage</th><th>消息指针</th></tr>
		<tr><th>u4Len</th><th>nPort</th><th>消息长度</th></tr>
		<tr><th>const char*</th><th>szIP</th><th>客户端UDP IP地址</th></tr>
		<tr><th>int</th><th>nPort</th><th>客户端UDP 端口</th></tr>
		<tr><th>bool</th><th>blHead</th><th>是否直接发送</th></tr>
		<tr><th>uint16</th><th>u2CommandID</th><th>发送的信令ID</th></tr>
		<tr><th>bool</th><th>blDlete</th><th>是否框架回收,true为框架回收,false为插件回收</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
</table>  

## 7.插件间调用方法（IModuleMessageManager）
### 说明:  
> 提供插件间的互相调用接口

###在插件中使用方法:
m_pServerObject->GetModuleMessageManager()  

###API列表：  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th width="20%">接口名称</th>
			<th width="20%">说明</th>
            <th width="60%" colspan="3">接口参数</th>
        </tr>
        <tr>
            <th rowspan="6">SendModuleMessage</th>
            <th rowspan="6">发送消息给指定的插件(同步调用)</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>const char*</th><th>pModuleName</th><th>被调用插件的名称</th></tr>
		<tr><th>uint16</th><th>u2CommandID</th><th>调用消息的信令ID</th></tr>
		<tr><th>IBuffPacket*</th><th>pBuffPacket</th><th>发送数据包信息</th></tr>
		<tr><th>IBuffPacket*</th><th>pReturnBuffPacket</th><th>接收数据包信息</th></tr>
		<tr><th>int</th><th>返回值</th><th>返回执行状态</th></tr>
        <tr>
            <th rowspan="6">SendModuleMessage</th>
            <th rowspan="6">发送消息给指定的插件(异步调用)</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>u2CommandID</th><th>调用消息的信令ID</th></tr>
		<tr><th>uint32</th><th>u4ConnectID</th><th>调用的ConnectID</th></tr>
		<tr><th>IBuffPacket*</th><th>pBuffPacket</th><th>发送数据包信息</th></tr>
		<tr><th>IBuffPacket*</th><th>pReturnBuffPacket</th><th>接收数据包信息</th></tr>
		<tr><th>int</th><th>返回值</th><th>返回执行状态</th></tr>
</table>  

## 8.插件开启关闭TCP服务监听（IControlListen）
### 说明:  
> 插件开启关闭TCP服务监听接口

###在插件中使用方法:
m_pServerObject->GetControlListen()  

###API列表：  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th width="20%">接口名称</th>
			<th width="20%">说明</th>
            <th width="60%" colspan="3">接口参数</th>
        </tr>
        <tr>
            <th rowspan="6">AddListen</th>
            <th rowspan="6">开启一个新的TCP监听</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>const char*</th><th>pListenIP</th><th>设置监听端口IP</th></tr>
		<tr><th>uint32</th><th>u4Port</th><th>设置监听端口</th></tr>
		<tr><th>uint8</th><th>u1IPType</th><th>监听端口是IPV4还是IPV6</th></tr>
		<tr><th>int</th><th>nPacketParseID</th><th>对应解析类ID,此ID对应main.xml中的PacketParse设置</th></tr>
		<tr><th>int</th><th>返回值</th><th>返回执行状态</th></tr>
        <tr>
            <th rowspan="4">DelListen</th>
            <th rowspan="4">关闭一个新的TCP监听</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>const char*</th><th>pListenIP</th><th>设置监听端口IP</th></tr>
		<tr><th>uint32</th><th>u4Port</th><th>设置监听端口</th></tr>
		<tr><th>int</th><th>返回值</th><th>返回执行状态</th></tr>
        <tr>
            <th rowspan="2">GetListenCount</th>
            <th rowspan="2">得到监听TCP端口的个数</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>int</th><th>返回值</th><th>当前监听数量</th></tr>
        <tr>
            <th rowspan="4">ShowListen</th>
            <th rowspan="4">查看监听端口信息</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint32</th><th>u4Index</th><th>当前监听对象下标,范围为GetListenCount接口返回的个数</th></tr>
		<tr><th>_ControlInfo&</th><th>objControlInfo</th><th>当前监听对象信息</th></tr>
		<tr><th>bool</th><th>返回值</th><th>返回执行状态</th></tr>
        <tr>
            <th rowspan="2">GetServerID</th>
            <th rowspan="2">得到当前服务器ID</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint32</th><th>返回值</th><th>得到当前服务器ID</th></tr>
</table>  

## 9.服务器插件管理（IModuleInfo）
### 说明:  
> 获得服务器的插件管理

###在插件中使用方法:
m_pServerObject->GetModuleInfo()  

###API列表：  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th width="20%">接口名称</th>
			<th width="20%">说明</th>
            <th width="60%" colspan="3">接口参数</th>
        </tr>
        <tr>
            <th rowspan="3">GetModuleExist</th>
            <th rowspan="3">判断一个插件是否已经被框架加载</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>const char*</th><th>pModuleName</th><th>插件名字</th></tr>
		<tr><th>bool</th><th>返回值</th><th>返回加载状态</th></tr>
        <tr>
            <th rowspan="3">GetModuleParam</th>
            <th rowspan="3">获得一个插件的初始化参数</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>const char*</th><th>pModuleName</th><th>插件名字</th></tr>
		<tr><th>char*</th><th>返回值</th><th>获得一个插件的初始化参数</th></tr>
        <tr>
            <th rowspan="3">GetModuleFileName</th>
            <th rowspan="3">获得一个插件的加载名称</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>const char*</th><th>pModuleName</th><th>插件名字</th></tr>
		<tr><th>char*</th><th>返回值</th><th>获得一个插件的加载名称</th></tr>
        <tr>
            <th rowspan="3">GetModuleFilePath</th>
            <th rowspan="3">获得一个插件的加载路径</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>const char*</th><th>pModuleName</th><th>插件名字</th></tr>
		<tr><th>char*</th><th>返回值</th><th>获得一个插件的加载路径</th></tr>
        <tr>
            <th rowspan="3">GetModuleFileDesc</th>
            <th rowspan="3">获得一个插件的描述信息</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>const char*</th><th>pModuleName</th><th>插件名字</th></tr>
		<tr><th>char*</th><th>返回值</th><th>获得一个插件的描述信息</th></tr>
</table>

## 10.ACE消息块管理（IMessageBlockManager）
### 说明:  
> ACEMessageBlock对象管理接口

###在插件中使用方法:
m_pServerObject->GetMessageBlockManager()  

###API列表：  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th width="20%">接口名称</th>
			<th width="20%">说明</th>
            <th width="60%" colspan="3">接口参数</th>
        </tr>
        <tr>
            <th rowspan="3">Create</th>
            <th rowspan="3">创建一个ACE消息块</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint32</th><th>u4Size</th><th>创建消息块大小</th></tr>
		<tr><th>ACE_Message_Block*</th><th>返回值</th><th>返回消息快指针</th></tr>
        <tr>
            <th rowspan="3">Close</th>
            <th rowspan="3">回收一个指定的ACE消息块</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>ACE_Message_Block*</th><th>pMessageBlock</th><th>消息块指针</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否回收成功</th></tr>
</table>    

## 11.框架维护指令模块（IFrameCommand）
### 说明:  
> 允许插件使用维护指令对框架进行控制

###在插件中使用方法:
m_pServerObject->GetFrameCommand()  

###API列表：  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th width="20%">接口名称</th>
			<th width="20%">说明</th>
            <th width="60%" colspan="3">接口参数</th>
        </tr>
        <tr>
            <th rowspan="3">DoFrameCommand</th>
            <th rowspan="3">执行一个维护指令</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>const char*</th><th>pCommand</th><th>维护指令</th></tr>
		<tr><th>IBuffPacket*</th><th>pBuffPacket</th><th>维护指令返回信息</th></tr>
		<tr><th>int</th><th>返回值</th><th>是否执行成功</th></tr>
</table>    


## 12.TTy接口（ITTyClientManager）
### 说明:  
> 提供插件访问串口，蓝牙，红外设备的接口。

###在插件中使用方法:
m_pServerObject->GetTTyClientManager()  

###API列表：  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th width="20%">接口名称</th>
			<th width="20%">说明</th>
            <th width="60%" colspan="3">接口参数</th>
        </tr>
        <tr>
            <th rowspan="6">Connect</th>
            <th rowspan="6">连接一个TTy设备</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>u2ConnectID</th><th>连接ID</th></tr>
		<tr><th>const char*</th><th>pName</th><th>设备名称</th></tr>
		<tr><th>_TTyDevParam</th><th>inParam</th><th>设备连接参数</th></tr>
		<tr><th>ITTyMessage*</th><th>pMessageRecv</th><th>设备回调接口类</th></tr>
		<tr><th>int</th><th>返回值</th><th>是否执行成功</th></tr>
        <tr>
            <th rowspan="4">GetClientDevInfo</th>
            <th rowspan="4">获得指定连接设备ID参数</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>u2ConnectID</th><th>连接ID</th></tr>
		<tr><th>_TTyDevParam&</th><th>outParam</th><th>连接设备参数</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
        <tr>
            <th rowspan="3">IsConnect</th>
            <th rowspan="3">查看设备是否连接</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>u2ConnectID</th><th>连接ID</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
        <tr>
            <th rowspan="3">Close</th>
            <th rowspan="3">关闭指定的一个设备连接</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>u2ConnectID</th><th>连接ID</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
        <tr>
            <th rowspan="3">Pause</th>
            <th rowspan="3">暂停指定的一个设备连接</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>u2ConnectID</th><th>连接ID</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
        <tr>
            <th rowspan="3">Resume</th>
            <th rowspan="3">恢复指定的一个设备连接</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>u2ConnectID</th><th>连接ID</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
        <tr>
            <th rowspan="5">SendMessage</th>
            <th rowspan="5">向指定的设备发送数据</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>u2ConnectID</th><th>连接ID</th></tr>
		<tr><th>char*&</th><th>pMessage</th><th>发送数据指针</th></tr>
		<tr><th>uint32</th><th>u4Len</th><th>发送数据长度</th></tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
</table>    


## 13.TMS计时器服务（TMService）
### 说明:  
> 提供集成与框架C++11实现的非ACE的计时器模块。

###在插件中使用方法:
1.使用方式：m_pServerObject->GetTMService()  
2.配合配置文件time.xml来使用
3.使用例子请查看CppUnit目录下Unit_TMService
###API列表：  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th width="20%">接口名称</th>
			<th width="20%">说明</th>
            <th width="60%" colspan="3">接口参数</th>
        </tr>
        <tr>
            <th rowspan="9">AddMessage</th>
            <th rowspan="9">添加一个消息</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>std::string</th><th>strName</th><th>消息名</th></tr>
		<tr><th>unsigned long long</th><th>nMessagePos</th><th>消息位编号</th></tr>
		<tr><th>long</th><th>sec</th><th>时间(秒)</th></tr>
		<tr><th>long</th><th>usec</th><th>时间(毫秒)</th></tr>
		<tr><th>int</th><th>_Message_id</th><th>消息编号(同time.xml中配置)</th></tr>
		<tr><th>void*</th><th>_arg</th><th>消息参数(注意参数的生命周期)</th></tr>
		<tr><th>Enum_Timer_Mode</th><th>emTimerMode</th><th>消息投递的方式</th></tr>
		<tr><th>int</th><th>返回值</th><th>返回0为行成功</th></tr>
        <tr>
            <th rowspan="4">DeleteMessage</th>
            <th rowspan="4">删除一个消息</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>std::string</th><th>strName</th><th>消息名</th></tr>
		<tr><th>unsigned long long</th><th>nMessagePos</th><th>消息位编号</th></tr>
		<tr><th>void*</th><th>返回值</th><th>删除消息后，返回加入消息时候的参数地址</th></tr>
        <tr>
            <th rowspan="2">GetRun</th>
            <th rowspan="2">是否在运行中</th>
            <th>类型</th>
            <th>参数名称</th>
            <th>说明</th>
        </tr>
		<tr><th>bool</th><th>返回值</th><th>是否执行成功</th></tr>
        <tr>
</table>    