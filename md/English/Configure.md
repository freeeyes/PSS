# PSS框架配置文件  
> PSS所有的配置文件都是XML格式的，里面包括运行框架的所有参数。   
> 你可以通过根据你的需要修改这些参数，达到控制框架的目的。   
> 目前支持的是，main.xml,logger.xml,forbiddenIP.xml,alert.xml,FileTestCfg.xml,ConsoleCommand.xml。  

## mian.xml(主框架配置信息)  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>标签</th>
			<th>标签说明</th>
            <th colspan="2">属性信息</th>
        </tr>
        <tr>
            <th rowspan="5">NetWorkMode</th>
            <th rowspan="5">框架网络模式配置</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>Mode</th><th>网络模式，目前支持Windows为IOCP,Select;Linux下为Epoll,Select,poll,Epoll_et</th></tr>
		<tr><th>BackLog</th><th>socket接收等待队列的长度</th></tr>
		<tr><th>ByteOrder</th><th>服务器字序，BuffPacket会按照这个字序来解析数据包中的数字，参数为HOST_ORDER或NET_ORDER</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="8">ServerType</th>
            <th rowspan="8">服务基本信息</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>Type</th><th>设置0为前台运行，1为后台运行(在windows下，1为以服务模式启动)</th></tr>
		<tr><th>name</th><th>当前服务的名称(仅windows服务状态下使用)</th></tr>
		<tr><th>displayname</th><th>当前服务器的显示名称(仅windows服务状态下使用)</th></tr>
		<tr><th>Debug</th><th>是否开启DEBUG模式,0位关闭，1为开启，开启后框架将额外输出数据运行日志</th></tr>
		<tr><th>DebugSize</th><th>如果DEBUG开启，这里将设置单条日志的最长输出长度</th></tr>
		<tr><th>IsClose</th><th>是否支持远程关闭服务,0为不支持，1为支持</th></tr>	
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="3">ServerID</th>
            <th rowspan="3">服务器ID</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>id</th><th>服务器的ID</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="3">ServerName</th>
            <th rowspan="3">服务器名称</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>name</th><th>服务器的名称</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="3">ServerVersion</th>
            <th rowspan="3">服务器版本号</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>Version</th><th>版本号</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="5">TCPServerIP</th>
            <th rowspan="5">服务器监听IP(可以是多个)</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>ip</th><th>监听IP，默认是INADDR_ANY</th></tr>
		<tr><th>port</th><th>监听端口</th></tr>
		<tr><th>ipType</th><th>IP地址模式，支持IPV4和IPV6</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="7">PacketParse</th>
            <th rowspan="7">数据包解析插件位置(可以是多个)</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>ParseID</th><th>Parse的ID</th></tr>
		<tr><th>ModulePath</th><th>插件的路径</th></tr>
		<tr><th>ModuleName</th><th>插件的文件名</th></tr>
		<tr><th>Type</th><th>插件的类型，目前支持数据包头包体模式H&B，以及数据流模式STREAM</th></tr>
		<tr><th>OrgLength</th><th>如果是H&B指的是数据包头长度，如果是STREAM模式指的是最大数据包长度</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="5">Console</th>
            <th rowspan="5">框架控制指令接收端口</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>support</th><th>0是开放端口，1是不开放端口</th></tr>
		<tr><th>sip</th><th>监控IP地址，默认是INADDR_ANY</th></tr>
		<tr><th>sport</th><th>监控的端口</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="8">Message</th>
            <th rowspan="8">消息工作线程控制相关参数</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>Msg_High_mark</th><th>工作线程队列的高水位标</th></tr>
		<tr><th>Msg_Low_mark</th><th>工作线程队列低水位标</th></tr>
		<tr><th>Msg_Buff_Max_Size</th><th>工作线程队列待处理命令最大容纳字节数</th></tr>
		<tr><th>Msg_Process</th><th>进程模式运行工作线程，数字代表启动的工作进程数量</th></tr>
		<tr><th>Msg_Thread</th><th>启动的工作线程数量</th></tr>
		<tr><th>Msg_MaxQueue</th><th>工作线程消息队列允许的最大未处理消息数</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="3">ModuleManager</th>
            <th rowspan="3">逻辑插件模块最大数量</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>MaxCount</th><th>逻辑插件模块最大数量</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="5">ModuleInfo</th>
            <th rowspan="5">逻辑插件模块信息(可以是多条)</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>ModuleSPath</th><th>逻辑插件模块路径</th></tr>
		<tr><th>ModuleSName</th><th>逻辑插件模块名</th></tr>
		<tr><th>ModuleSParam</th><th>逻辑插件启动参数</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="3">CoreSetting</th>
            <th rowspan="3">Coredump文件生成配置(只适用于Linux)</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>CoreNeed</th><th>0是关闭，1是打开</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="5">CommandAccount</th>
            <th rowspan="5">信令访问统计日志记录</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>Account</th><th>数据包按信令个数统计，0是关闭，1是打开</th></tr>
		<tr><th>FlowAccount</th><th>数据包按信令流量统计，0是关闭，1是打开</th></tr>
		<tr><th>MaxCommandCount</th><th>支持最大信令列表个数</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="4">RecvInfo</th>
            <th rowspan="4">接收数据包配置相关</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>RecvBuffSize</th><th>设置Socket接收缓冲区大小</th></tr>
		<tr><th>RecvQueueTimeout</th><th>设置接收Socket超时时间</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="10">SendInfo</th>
            <th rowspan="10">发送数据队列配置相关</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>SendQueueMax</th><th>发送数据队列最大发送数据数量</th></tr>
		<tr><th>TcpNodelay</th><th>设置Socket的Nodelay算法参数</th></tr>
		<tr><th>MaxBlockSize</th><th>发送最大数据包长度</th></tr>
		<tr><th>BlockCount</th><th>发送缓冲池中能缓冲数据包的最大个数</th></tr>
		<tr><th>SendTimeout</th><th>Socket发送超时时间(单位毫秒)</th></tr>
		<tr><th>SendQueueTimeout</th><th>发送队列的最大写入时间</th></tr>
		<tr><th>SendQueueCount</th><th>发送队列的数量</th></tr>
		<tr><th>PutQueueTimeout</th><th>发送数据包放入发送队列的超时时间</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="6">ThreadInfo</th>
            <th rowspan="6">工作线程自检配置</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>ThreadTimeout</th><th>工作线程自检周期,单位是秒</th></tr>
		<tr><th>ThreadTimeCheck</th><th>工作线程判断执行死锁或者不响应的最长时间,单位是秒</th></tr>
		<tr><th>DisposeTimeout</th><th>工作线程处理时间超时告警</th></tr>
		<tr><th>PutQueueTimeout</th><th>数据放入工作线程中的最长超时时间</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="8">ThreadInfoAI</th>
            <th rowspan="8">工作线程根据信令处理时间过滤超时信令配置</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>AI</th><th>0是关闭，1是开启</th></tr>
		<tr><th>CheckTime</th><th>数据包处理时间检测周期，单位是秒</th></tr>
		<tr><th>TimeoutCount</th><th>数据包处理超时时间，单位是秒</th></tr>
		<tr><th>StopTime</th><th>当数据包连续超时，停止接收某信令数据包的时间，单位是秒</th></tr>
		<tr><th>ReturnDataType</th><th>当数据包连续处理超时，返回不处理数据包给客户端的指令类型，1位二进制，0为文本</th></tr>
		<tr><th>ReturnData</th><th>返回二进制文本信息数据包体</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="7">ClientInfo</th>
            <th rowspan="7">客户端接入连接管理配置</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>HandlerCount</th><th>数据连接允许的最大数量当前缓冲池大小</th></tr>
		<tr><th>MaxHandlerCount</th><th>数据连接允许的最大数量最大缓冲池大小</th></tr>
		<tr><th>MaxConnectTime</th><th>当数据链路连接上，超过这个时间不发送数据，就会自动断开连接，单位是秒</th></tr>
		<tr><th>CheckAliveTime</th><th>检测数据链路数据状态的周期时间，单位是秒</th></tr>
		<tr><th>MaxBuffRecv</th><th>接收数据包的最大大小</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="6">ConnectServer</th>
            <th rowspan="6">服务器间连接信息配置</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>TimeInterval</th><th>服务器间连接存活检测最长超时时间，单位是秒</th></tr>
		<tr><th>TimeCheck</th><th>自检周期时间，单位是秒</th></tr>
		<tr><th>Recvbuff</th><th>接收服务器间数据包的最大大小</th></tr>
		<tr><th>RunType</th><th>服务器间返回包处理模式，0为同步，1为异步</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="5">Monitor</th>
            <th rowspan="5">PSS框架监控信息</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>CpuAndMemory</th><th>CPU和内存监控，0是关闭，1是开启</th></tr>
		<tr><th>CpuMax</th><th>最大CPU的报警上限</th></tr>
		<tr><th>MemoryMax</th><th>最大内存使用的报警上限</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="8">AceDebug</th>
            <th rowspan="8">AceDebug设置输出</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>TrunOn</th><th>0是关闭，1是开启</th></tr>
		<tr><th>DebugName</th><th>DEBUG信息输出文件名</th></tr>
		<tr><th>ChkInterval</th><th>检测周期</th></tr>
		<tr><th>LogFileMaxSize</th><th>单个日志文件最大大小</th></tr>
		<tr><th>LogFileMaxCnt</th><th>日志文件循环覆盖的个数</th></tr>
		<tr><th>Level</th><th>日志输出等级</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
</table>

## logger.xml(框架日志配置信息)  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>标签</th>
			<th>标签说明</th>
            <th colspan="2">属性信息</th>
        </tr>
        <tr>
            <th rowspan="3">ServerLogHead</th>
            <th rowspan="3">服务器日志文件统一标头</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>Text</th><th>日志统一标头</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="3">LogPath</th>
            <th rowspan="3">日志文件夹路径</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>Path</th><th>日志文件夹路径</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="4">LogPool</th>
            <th rowspan="4">日志记录缓冲池相关配置</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>BlockSize</th><th>单个日志块大小</th></tr>
		<tr><th>PoolCount</th><th>日志缓冲池个数</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="3">LogLevel</th>
            <th rowspan="3">日志等级相关配置</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>CurrLevel</th><th>当然日志等级，只有大于此等级的日志会被记录，低于此等级的日志不会被记录</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="7">LogInfo</th>
            <th rowspan="7">具体日志信息(可以是多个)</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>logid</th><th>当前Log日志的ID，必须唯一</th></tr>
		<tr><th>logtype</th><th>日志类型，1为Operation文件夹日志，2为ServerError日志</th></tr>
		<tr><th>Display</th><th>是否显示输出，如果是选择显示，则不再输出到文件中，0为否，1为是</th></tr>
		<tr><th>Level</th><th>当前日志等级，可以按需要任意设定，和CurrLevel对应</th></tr>
		<tr><th>logname</th><th>日志名称，会一天生成一个</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
</table>
## forbiddenIP.xml(IP禁用配置信息)  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>标签</th>
			<th>标签说明</th>
            <th colspan="2">属性信息</th>
        </tr>
        <tr>
            <th rowspan="4">ForbiddenIP</th>
            <th rowspan="4">禁止访问的客户端IP列表(可以是多个)</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>ip</th><th>要禁止连接的客户端IP</th></tr>
		<tr><th>type</th><th>连接类型,TCP或者UDP</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
</table>
## alert.xml(框架报警配置信息)  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>标签</th>
			<th>标签说明</th>
            <th colspan="2">属性信息</th>
        </tr>
        <tr>
            <th rowspan="7">AlertConnect</th>
            <th rowspan="7">连接信息告警配置</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>ConnectMin</th><th>当前最小连接数(1分钟内)，如果是0，则不监控,超过这个数字则报警邮件</th></tr>
		<tr><th>ConnectMax</th><th>当前最大连接数(1分钟内)，如果是0，则不监控,超过这个数字则报警邮件</th></tr>
		<tr><th>DisConnectMin</th><th>当前最小断开连接数(1分钟内)，如果是0，则不监控,超过这个数字则报警邮件</th></tr>
		<tr><th>DisConnectMax</th><th>当前最大断开连接数(1分钟内)，如果是0，则不监控,超过这个数字则报警邮件</th></tr>
		<tr><th>ConnectAlert</th><th>当前存有的最大连接数，超过这个数字则报警邮件</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="4">IP</th>
            <th rowspan="4">客户端IP连接数监控报警</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>IPMax</th><th>同一个客户端IP连接的最大数量(单位时间内)，超过这个数字则报警邮件</th></tr>
		<tr><th>Timeout</th><th>检测周期，单位时间时长，单位是秒</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="7">ClientData</th>
            <th rowspan="7">单位时间，单个连接接收发送数据监控配置</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>RecvPacketCount</th><th>1分钟内接收单个连接的数据包个数最大阈值，超过这个数字则报警邮件</th></tr>
		<tr><th>RecvDataMax</th><th>1分钟内接收单个连接的数据包总大小最大阈值，超过这个数字则报警邮件</th></tr>
		<tr><th>SendPacketCount</th><th1分钟内发送单个连接的数据包个数最大阈值，超过这个数字则报警邮件</th></tr>
		<tr><th>SendDataMax</th><th>1分钟内发送单个连接的数据包总大小最大阈值，超过这个数字则报警邮件</th></tr>
		<tr><th>MailID</th><th>对应发送邮件信息的ID</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="5">CommandInfo</th>
            <th rowspan="5">信令监控配置信息(可以是多个)</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>CommandID</th><th>需要监控的信令ID/th></tr>
		<tr><th>CommandCount</th><th>1分钟内处理该信令的最大数字，如果是0则不监控，超过这个数字则报警邮件</th></tr>
		<tr><th>MailID</th><th>对应发送邮件信息的ID</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="8">Mail</th>
            <th rowspan="8">报警邮件相关配置(可以是多个)</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>MailID</th><th>报警邮件ID/th></tr>
		<tr><th>fromMailAddr</th><th>设置邮件发送者</th></tr>
		<tr><th>toMailAddr</th><th>设置邮件接受者</th></tr>
		<tr><th>MailPass</th><th>邮箱密码</th></tr>
		<tr><th>MailUrl</th><th>邮箱smtp地址</th></tr>
		<tr><th>MailPort</th><th>邮箱的smtp端口</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
</table>
## ConsoleCommand.xml(框架控制指令权限配置信息)  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>标签</th>
			<th>标签说明</th>
            <th colspan="2">属性信息</th>
        </tr>
        <tr>
            <th rowspan="4">CommandInfo</th>
            <th rowspan="4">PSS框架的后台命令权限配置(可以是多个)</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>CommandName</th><th>命令名称</th></tr>
		<tr><th>User</th><th>允许的用户信息，多个用户用逗号隔开</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
</table>
## FileTestCfg.xml(文件测试接口配置信息)  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>标签</th>
			<th>标签说明</th>
            <th colspan="2">属性信息</th>
        </tr>
        <tr>
            <th rowspan="10">FileTestConfig</th>
            <th rowspan="10">测试文件接口录入数据包配置</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>Path</th><th>测试文件数据包路径</th></tr>
		<tr><th>TimeInterval</th><th>测试时间周期，单位是秒</th></tr>
		<tr><th>ConnectCount</th><th>单位周期连接建立数量</th></tr>
		<tr><th>ResponseCount</th><th>单位周期数据包处理返回数量</th></tr>
		<tr><th>ExpectTime</th><th>期待测试总耗时，单位是毫秒</th></tr>
		<tr><th>ParseID</th><th>需要配合框架的解析器插件ID</th></tr>
		<tr><th>ContentType</th><th>连接类型，1是二进制数据包,0是文本数据包</th></tr>
		<tr><th>ContentType</th><th>连接类型，1是TCP,2是UDP</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
        <tr>
            <th rowspan="3">FileInfo</th>
            <th rowspan="3">测试文件包配置(可以是多个)</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>FileName</th><th>测试数据包文件名</th></tr>
		<tr><th>desc</th><th>当前标签描述信息</th></tr>
</table>