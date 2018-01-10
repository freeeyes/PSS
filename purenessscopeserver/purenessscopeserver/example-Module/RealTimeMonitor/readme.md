# 监控数据包发送格式  
> 目前支持两个指令。  
```cpp
#define COMMAND_MONITOR_LOGIN      0x2001    //发送注册命令
#define COMMAND_MONITOR_DATA       0x2002    //发送心跳当前数据吞吐命令
#define COMMAND_MONITOR_LOGIN_ACK  0x8001    //注册命令回应
#define COMMAND_MONITOR_DATA_ACK   0x8002    //当前数据吞吐命令回应
```

## 命令格式   
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>标签</th>
			<th>标签说明</th>
            <th colspan="2">属性信息</th>
        </tr>
        <tr>
            <th rowspan="7">COMMAND_MONITOR_LOGIN</th>
            <th rowspan="7">向监控服务器发注册指令</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>Version(uint16)</th><th>数据包版本</th></tr>
		<tr><th>CommandID(uint16)</th><th>数据包命令ID</th></tr>
		<tr><th>PacketBodyLength(uint32)</th><th>数据包体的总长度，不包含包头长度</th></tr>
		<tr><th>Session(char*32)</th><th>数据包Session字符串</th></tr>
		<tr><th>IP长度(uint16)</th><th>当前服务器IP长度</th></tr>
		<tr><th>IP(IP长度)</th><th>当前服务器IP字符串</th></tr>
        <tr>
            <th rowspan="4">COMMAND_MONITOR_LOGIN_ACK</th>
            <th rowspan="4">注册指令返回</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>包长(uint32)</th><th>数据包体的总长度，不包含包头长度</th></tr>
		<tr><th>命令字(uint16)</th><th>命令字</th></tr>
		<tr><th>接收状态(uint32)</th><th>接收状态</th></tr>
        <tr>
            <th rowspan="10">COMMAND_MONITOR_DATA</th>
            <th rowspan="10">向监控服务器发心跳指令</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>Version(uint16)</th><th>数据包版本</th></tr>
		<tr><th>CommandID(uint16)</th><th>数据包命令ID</th></tr>
		<tr><th>PacketBodyLength(uint32)</th><th>数据包体的总长度，不包含包头长度</th></tr>
		<tr><th>Session(char*32)</th><th>数据包Session字符串</th></tr>
		<tr><th>CpuRote(uint32)</th><th>当前CPU运行百分比，除以100获得百分率</th></tr>
		<tr><th>MemorySize(uint32)</th><th>当前内存占用量，单位是Mb</th></tr>
		<tr><th>ConnectCount(uint32)</th><th>当前服务器连接数</th></tr>
		<tr><th>DataInSize(uint32)</th><th>当前一分钟内流入字节数</th></tr>
		<tr><th>DataOutSize（uint32）</th><th>当前一分钟内流出字节数</th></tr>
        <tr>
            <th rowspan="4">COMMAND_MONITOR_DATA_ACK</th>
            <th rowspan="4">心跳数据返回</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>包长(uint32)</th><th>数据包体的总长度，不包含包头长度</th></tr>
		<tr><th>命令字(uint16)</th><th>命令字</th></tr>
		<tr><th>接收状态(uint32)</th><th>接收状态</th></tr>
</table>