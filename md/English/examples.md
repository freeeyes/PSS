# 样例逻辑插件工程说明  
## (1)TcpTest插件  
### 目的  
实现标准的TCP数据包处理流程展示。  
### 测试信令格式  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>测试信令ID</th>
			<th>信令说明</th>
            <th colspan="2">数据包信息</th>
        </tr>
        <tr>
            <th rowspan="7">COMMAND_BASE(0x1000)</th>
            <th rowspan="7">测试数据包结构</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string</th><th>数据包体内容</th></tr>
</table>  

## (2)UdpTest插件  
### 目的  
实现标准的TCP数据包处理流程展示。  
### 测试信令格式  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>测试信令ID</th>
			<th>信令说明</th>
            <th colspan="2">数据包信息</th>
        </tr>
        <tr>
            <th rowspan="7">COMMAND_BASE(0x1000)</th>
            <th rowspan="7">测试数据包结构</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string</th><th>数据包体内容</th></tr>
</table>  

## (3)AutoTest插件  
### 目的  
实现对PSS框架数据包完整性检测功能。  
### 测试信令格式  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>测试信令ID</th>
			<th>信令说明</th>
            <th colspan="2">数据包信息</th>
        </tr>
        <tr>
            <th rowspan="6">COMMAND_AUTOTEST_HEAD(0x5000)</th>
            <th rowspan="6">带头返回数据包</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>uint64</th><th>数据包时间戳</th></tr>
        <tr>
            <th rowspan="6">COMMAND_AUTOTEST_NOHEAD(0x5001)</th>
            <th rowspan="6">不带头返回数据包</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>uint64</th><th>数据包时间戳</th></tr>
        <tr>
            <th rowspan="6">COMMAND_AUTOTEST_HEADBUFF(0x5002)</th>
            <th rowspan="6">带头缓冲数据包</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>uint64</th><th>数据包时间戳</th></tr>
        <tr>
            <th rowspan="6">COMMAND_AUTOTEST_NOHEADBUFF(0x5003)</th>
            <th rowspan="6">不带头缓冲数据包</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>uint64</th><th>数据包时间戳</th></tr>
        <tr>
            <th rowspan="6">COMMAND_AUTOTEST_LOGDATA(0x5004)</th>
            <th rowspan="6">测试记录日志</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>uint64</th><th>数据包时间戳</th></tr>
        <tr>
            <th rowspan="6">COMMAND_AUTOTEST_WORKAI(0x5006)</th>
            <th rowspan="6">测试workAI触发机制</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>uint64</th><th>数据包时间戳</th></tr>
</table>  

## (4)FtpTest插件  
### 目的  
实现对PSS框架简单FTP浏览和下载功能。  
### 测试信令格式  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>测试信令ID</th>
			<th>信令说明</th>
            <th colspan="2">数据包信息</th>
        </tr>
        <tr>
            <th rowspan="9">COMMAND_LOGIN(0x2000)</th>
            <th rowspan="9">登录</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>uint8</th><th>登录名长度</th></tr>
		<tr><th>string</th><th>登录名</th></tr>
		<tr><th>uint8</th><th>密码长度</th></tr>
		<tr><th>string</th><th>密码</th></tr>
        <tr>
            <th rowspan="7">COMMAND_LOGOUT(0x2001)</th>
            <th rowspan="7">退出</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>uint8</th><th>登录名长度</th></tr>
		<tr><th>string</th><th>登录名</th></tr>
        <tr>
            <th rowspan="9">COMMAND_FILELIST(0x2002)</th>
            <th rowspan="9">文件，文件夹列表</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>uint8</th><th>登录名长度</th></tr>
		<tr><th>string</th><th>登录名</th></tr>
		<tr><th>uint8</th><th>当前路径长度</th></tr>
		<tr><th>string</th><th>当前路径</th></tr>
        <tr>
            <th rowspan="11">COMMAND_FILE_DOWNLOAD(0x2003)</th>
            <th rowspan="11">不带头缓冲数据包</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>uint8</th><th>登录名长度</th></tr>
		<tr><th>string</th><th>登录名</th></tr>
		<tr><th>uint8</th><th>当前文件名长度</th></tr>
		<tr><th>string</th><th>当前文件名</th></tr>
		<tr><th>uint32</th><th>数据包长度</th></tr>
		<tr><th>uint32</th><th>数据包序号，第一个是0</th></tr>
        <tr>
            <th rowspan="14">COMMAND_FILE_UPLOAD(0x2004)</th>
            <th rowspan="14">文件上传</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>uint64</th><th>数据包时间戳</th></tr>
		<tr><th>uint8</th><th>登录名长度</th></tr>
		<tr><th>string</th><th>登录名</th></tr>
		<tr><th>uint8</th><th>当前文件名长度</th></tr>
		<tr><th>string</th><th>当前文件名</th></tr>
		<tr><th>uint32</th><th>数据包长度</th></tr>
		<tr><th>uint32</th><th>数据包序号，第一个是0</th></tr>
		<tr><th>uint32</th><th>文件块长度</th></tr>
		<tr><th>string</th><th>文件块</th></tr>
</table>  

## (5)Http插件  
### 目的  
实现对PSS的Http命令解析支持。  
### 测试信令格式  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>测试信令ID</th>
			<th>信令说明</th>
            <th colspan="2">数据包信息</th>
        </tr>
        <tr>
            <th rowspan="6">COMMAND_HTTP_DATA(0xea01)</th>
            <th rowspan="6">Http数据请求协议数据包</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>string</th><th>Http数据包体内容</th></tr>
</table>  

## (6)kafka插件  
### 目的  
实现对PSS框架对Kafka接口的支持。  
### 测试信令格式  
无接收指令，这个功能是演示kafka的消费者线程和PSS的整合，收到kafka的消费信息并下发。

## (7)LoginBuffer插件  
### 目的  
实现对PSS与共享内存互动，通过共享内存实现用户的读写。  
### 测试信令格式  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>测试信令ID</th>
			<th>信令说明</th>
            <th colspan="2">数据包信息</th>
        </tr>
        <tr>
            <th rowspan="9">COMMAND_LOGIN(0x2100)</th>
            <th rowspan="9">用户登录</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>uint8</th><th>登录名长度</th></tr>
		<tr><th>string</th><th>登录名</th></tr>
		<tr><th>uint8</th><th>密码长度</th></tr>
		<tr><th>string</th><th>密码</th></tr>
        <tr>
            <th rowspan="7">COMMAND_LOGOUT(0x2101)</th>
            <th rowspan="7">用户退出</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>uint8</th><th>登录名长度</th></tr>
		<tr><th>string</th><th>登录名</th></tr>
        <tr>
            <th rowspan="6">COMMAND_USERINFO(0x2102)</th>
            <th rowspan="6">查询用户信息</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>uint32</th><th>登录名ID</th></tr>
        <tr>
            <th rowspan="8">COMMAND_SET_USERINFO(0x2103)</th>
            <th rowspan="8">设置用户信息</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>uint32</th><th>登录名ID</th></tr>
		<tr><th>uint32</th><th>用户血量</th></tr>
		<tr><th>uint32</th><th>用户魔法</th></tr>
</table> 

## (8)LoginBuffer插件  
### 目的  
实现对PSS与共享内存互动，通过共享内存实现用户的读写。  
### 测试信令格式  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>测试信令ID</th>
			<th>信令说明</th>
            <th colspan="2">数据包信息</th>
        </tr>
        <tr>
            <th rowspan="9">COMMAND_LOGIN(0x2100)</th>
            <th rowspan="9">用户登录</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>uint8</th><th>登录名长度</th></tr>
		<tr><th>string</th><th>登录名</th></tr>
		<tr><th>uint8</th><th>密码长度</th></tr>
		<tr><th>string</th><th>密码</th></tr>
        <tr>
            <th rowspan="7">COMMAND_LOGOUT(0x2101)</th>
            <th rowspan="7">用户退出</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>uint8</th><th>登录名长度</th></tr>
		<tr><th>string</th><th>登录名</th></tr>
        <tr>
            <th rowspan="6">COMMAND_USERINFO(0x2102)</th>
            <th rowspan="6">查询用户信息</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>uint32</th><th>登录名ID</th></tr>
        <tr>
            <th rowspan="8">COMMAND_SET_USERINFO(0x2103)</th>
            <th rowspan="8">设置用户信息</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>uint32</th><th>登录名ID</th></tr>
		<tr><th>uint32</th><th>用户血量</th></tr>
		<tr><th>uint32</th><th>用户魔法</th></tr>
</table> 

## (9)LuaTest插件  
### 目的  
实现对PSS与Lua脚本互相调用。  
### 测试信令格式  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>测试信令ID</th>
			<th>信令说明</th>
            <th colspan="2">数据包信息</th>
        </tr>
        <tr>
            <th rowspan="6">COMMAND_BASE(0x1000)</th>
            <th rowspan="6">用户登录</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>string</th><th>数据参数</th></tr>
</table> 

## (10)PlugAtoB插件  
### 目的  
实现对PSS的插件间互相调用。  
### 测试信令格式  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>测试信令ID</th>
			<th>信令说明</th>
            <th colspan="2">数据包信息</th>
        </tr>
        <tr>
            <th rowspan="6">COMMAND_PLUGA(0x2201)</th>
            <th rowspan="6">实现插件间消息通知</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>uint8</th><th>数据参数长度</th></tr>
		<tr><th>string</th><th>数据参数</th></tr>
</table>  

## (11)Proxy插件  
### 目的  
实现对PSS的客户端与服务器之间消息转发
### 测试信令格式  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>测试信令ID</th>
			<th>信令说明</th>
            <th colspan="2">数据包信息</th>
        </tr>
        <tr>
            <th rowspan="6">COMMAND_PROXY(0x1030)</th>
            <th rowspan="6">需要转发到其他服务器的消息</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>string</th><th>数据参数</th></tr>
</table>  

## (12)TestTCPPost插件  
### 目的  
实现对PSS的TCP数据透传
### 测试信令格式  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>测试信令ID</th>
			<th>信令说明</th>
            <th colspan="2">数据包信息</th>
        </tr>
        <tr>
            <th rowspan="6">COMMAND_PROXY(0x1030)</th>
            <th rowspan="6">需要转发到其他服务器的消息</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>string</th><th>数据参数</th></tr>
</table>  

## (13)TestUDPPost插件  
### 目的  
实现对PSS的UDP数据透传
### 测试信令格式  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>测试信令ID</th>
			<th>信令说明</th>
            <th colspan="2">数据包信息</th>
        </tr>
        <tr>
            <th rowspan="6">COMMAND_PROXY(0x1030)</th>
            <th rowspan="6">需要转发到其他服务器的消息</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>string</th><th>数据参数</th></tr>
</table>  

## (14)TimerTest插件  
### 目的  
实现对PSS的定时器调用插件
### 测试信令格式  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>测试信令ID</th>
			<th>信令说明</th>
            <th colspan="2">数据包信息</th>
        </tr>
        <tr>
            <th rowspan="6">COMMAND_TIMER(0x1100)</th>
            <th rowspan="6">定时器调用指令</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>string</th><th>数据参数</th></tr>
</table>  


## (15)WebSocket插件  
### 目的  
实现对PSS的websocket消息解析
### 测试信令格式  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>测试信令ID</th>
			<th>信令说明</th>
            <th colspan="2">数据包信息</th>
        </tr>
        <tr>
            <th rowspan="6">COMMAND_HANDIN(0xee01)</th>
            <th rowspan="6">websocket消息头验证</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>string</th><th>数据参数</th></tr>
        <tr>
            <th>测试信令ID</th>
			<th>信令说明</th>
            <th colspan="2">数据包信息</th>
        </tr>
        <tr>
            <th rowspan="6">COMMAND_DATAIN(0xee02)</th>
            <th rowspan="6">websocket消息体处理</th>
            <th>属性</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>数据包版本号</th></tr>
		<tr><th>uint16</th><th>数据包命令ID</th></tr>
		<tr><th>uint32</th><th>数据包体长度</th></tr>
		<tr><th>string(32 chars)</th><th>数据包Session</th></tr>
		<tr><th>string</th><th>数据参数</th></tr>
</table>  

## (15)ZooClient插件  
### 目的  
实现对PSS对zookeeper消息的调用
### 测试信令格式  
无数据包，只是实现zookeeper的发现功能。  