# PSS框架命令指令集  
## 1.命令组成  
PSS框架支持对框架内部运行状态操作的指令集。    
这些指令包括框架的插件管理，工作线程状态管理，框架基本参数查询，连接数列表，服务器端口管理，IP管理等等。

命令格式为:  
<命令输出格式> <用户名> <命令字> <参数><命令结束符>  
例如:  
b freeeyes ShowServerInfo -a&  
**b**: 输出结果以二进制结果输出，如果想以文本结构输出，这里可以使用 **i** (比如在telnet下)  
**freeeyes**: 用户名，这个是必须的，PSS框架会对用户进行校验，具体校验设置在**main.xml**  （设置用户名）和**ConsoleCommand.xml**  （设置对应的权限）有权限才能被执行。    
**ShowServerInfo**: 命令的名称.  
**-a**: 命令的参数部分。  
**&**: 命令结束符，是必须的。

##2.命令说明(以二进制结果输出返回)   
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th width="20%">命令样例</th>
			<th width="20%">说明</th>
            <th width="60%" colspan="2">返回数据结构</th>
        </tr>
        <tr>
            <th rowspan="11">ShowServerInfo -a</th>
            <th rowspan="11">获得框架基本运行参数</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
		<tr><th>uint16</th><th>服务器ID</th></tr>
		<tr><th>uint8</th><th>服务器名称字符串长度</th></tr>
		<tr><th>char</th><th>服务器名称</th></tr>
		<tr><th>uint8</th><th>服务版本字符串长度</th></tr>
		<tr><th>char</th><th>服务器版本</th></tr>
		<tr><th>uint16</th><th>服务器插件个数</th></tr>
		<tr><th>uint16</th><th>服务器工作线程个数</th></tr>
		<tr><th>uint16</th><th>服务器协议包版本号</th></tr>
		<tr><th>uint8</th><th>服务器字序大端还是小段，0小端 1大端</th></tr>
		<tr><th>uint8</th><th>服务器字序 0 主机序 1 网络序</th></tr>
        <tr>
            <th rowspan="16">ShowModule -a</th>
            <th rowspan="16">查看所有框架正在运行的插件</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
		<tr><th>uint32</th><th>当前运行插件个数</th></tr>
		<tr><th>uint8</th><th>插件文件名长度</th></tr>
		<tr><th>char</th><th>插件文件名</th></tr>
		<tr><th>uint8</th><th>插件名称长度</th></tr>
		<tr><th>char</th><th>插件名称</th></tr>
		<tr><th>uint8</th><th>插件路径长度</th></tr>
		<tr><th>char</th><th>插件路径</th></tr>
		<tr><th>uint8</th><th>插件启动参数长度</th></tr>
		<tr><th>char</th><th>插件启动参数</th></tr>
		<tr><th>uint8</th><th>插件描述长度</th></tr>
		<tr><th>char</th><th>插件描述</th></tr>
		<tr><th>uint8</th><th>插件加载时间长度</th></tr>
		<tr><th>char</th><th>插件加载时间</th></tr>
		<tr><th>uint8</th><th>插件当前运行状态字 0 为正常</th></tr>
		<tr><th>uint32</th><th>插件当前运行错误码 0 为正常</th></tr>
        <tr>
            <th rowspan="2">LoadModule TcpTest.so,./,Param&</th>
            <th rowspan="2">加载一个插件,TcpTest.so是文件名,./是插件在服务器上的路径，Param是插件启动参数</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint8</th><th>插件启动结果 0 成功 1 失败</th></tr>
        <tr>
            <th rowspan="2">UnLoadModule TestTcp&</th>
            <th rowspan="2">卸载一个插件，TestTcp是插件自己的名字(注意不是文件名,是设置在插件内的名称)</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint8</th><th>插件启动结果 0 成功 1 失败</th></tr>
        <tr>
            <th rowspan="2">ReLoadModule TestTcp&</th>
            <th rowspan="2">重载一个插件，TestTcp是插件自己的名字(注意不是文件名,是设置在插件内的名称)</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint8</th><th>插件启动结果 0 成功 1 失败</th></tr>
        <tr>
            <th rowspan="4">ClientCount -cp&</th>
            <th rowspan="4">当前客户端连接数</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
		<tr><th>uint32</th><th>当前客户端连接总数</th></tr>
		<tr><th>uint32</th><th>当前客户端连接池剩余总数</th></tr>
		<tr><th>uint32</th><th>当前客户端连接池总数</th></tr>
        <tr>
            <th rowspan="6">CommandInfo 0x1000&</th>
            <th rowspan="6">查询指定信令信息, 0x1000为信令十六进制ID</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
		<tr><th>uint32</th><th>信令个数</th></tr>
		<tr><th>uint16</th><th>当前信令个数</th></tr>
		<tr><th>uint16</th><th>信令ID</th></tr>
		<tr><th>uint32</th><th>信令已执行次数</th></tr>
		<tr><th>uint64</th><th>信令已执行总分时间，单位毫秒</th></tr>
        <tr>
            <th rowspan="5">CommandTimeout -a&</th>
            <th rowspan="5">查询所有超时信令列表</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
		<tr><th>uint32</th><th>超时信令个数</th></tr>
		<tr><th>uint16</th><th>超时信令ID</th></tr>
		<tr><th>uint32</th><th>超时信令发生时间</th></tr>
		<tr><th>uint32</th><th>运行时间，单位是秒</th></tr>
        <tr>
            <th rowspan="2">CommandTimeoutclr -a&</th>
            <th rowspan="2">清空超时信令列表</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint8</th><th>执行状态 0 成功</th></tr>
        <tr>
            <th rowspan="2">CommandDataLog -a&</th>
            <th rowspan="2">存储CommandDataLog</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint8</th><th>执行状态 0 成功</th></tr>
        <tr>
            <th rowspan="10">WorkThreadState -s&</th>
            <th rowspan="10">查看工作线程状态</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
		<tr><th>uint8</th><th>工作线程个数</th></tr>
		<tr><th>uint8</th><th>工作线程ID</th></tr>
		<tr><th>uint32</th><th>工作线程最后运行时间，格式为1970-01-01到现在的秒值</th></tr>
		<tr><th>uint32</th><th>工作线程启动时间，格式为1970-01-01到现在的秒值</th></tr>
		<tr><th>uint8</th><th>工作线程当前状态</th></tr>
		<tr><th>uint32</th><th>工作线程当前处理数据包总数</th></tr>
		<tr><th>uint16</th><th>工作线程当前运行信令ID</th></tr>
		<tr><th>uint16</th><th>工作线程平均包处理时间,单位是毫秒</th></tr>
		<tr><th>uint16</th><th>工作线程最近一分钟处理的数据包数量</th></tr>
        <tr>
            <th rowspan="14">ConnectClient -a&</th>
            <th rowspan="14">当前TCP客户端链接的信息</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint32</th><th>当前连接数总数</th></tr>
		<tr><th>uint8</th><th>连接名称字符串长度</th></tr>
		<tr><th>char</th><th>连接名称字符串，默认是连接的客户端IP,也可以自己设置</th></tr>
		<tr><th>uint32</th><th>连接ID</th></tr>
		<tr><th>uint32</th><th>连接接收包总数</th></tr>
		<tr><th>uint32</th><th>连接发送包总数</th></tr>
		<tr><th>uint32</th><th>连接接收包总字节</th></tr>
		<tr><th>uint32</th><th>连接发送包总字节</th></tr>
		<tr><th>uint32</th><th>连接创建时间,格式为1970-01-01到现在的秒值</th></tr>
		<tr><th>uint32</th><th>连接活跃时间,单位是秒值</th></tr>
		<tr><th>uint32</th><th>连接接收队列数据包总数</th></tr>
		<tr><th>uint64</th><th>连接接收总消耗时间</th></tr>
		<tr><th>uint64</th><th>连接发送总消耗时间</th></tr>
        <tr>
            <th rowspan="14">UDPConnectClient -a&</th>
            <th rowspan="14">当前UDP客户端链接的信息</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint32</th><th>当前连接数总数</th></tr>
		<tr><th>uint8</th><th>连接名称字符串长度</th></tr>
		<tr><th>char</th><th>连接名称字符串，默认是连接的客户端IP,也可以自己设置</th></tr>
		<tr><th>uint32</th><th>连接ID</th></tr>
		<tr><th>uint32</th><th>连接接收包总数</th></tr>
		<tr><th>uint32</th><th>连接发送包总数</th></tr>
		<tr><th>uint32</th><th>连接接收包总字节</th></tr>
		<tr><th>uint32</th><th>连接发送包总字节</th></tr>
		<tr><th>uint32</th><th>连接创建时间,格式为1970-01-01到现在的秒值</th></tr>
		<tr><th>uint32</th><th>连接活跃时间,单位是秒值</th></tr>
		<tr><th>uint32</th><th>连接接收队列数据包总数</th></tr>
		<tr><th>uint64</th><th>连接接收总消耗时间</th></tr>
		<tr><th>uint64</th><th>连接发送总消耗时间</th></tr>
        <tr>
            <th rowspan="2">CloseClient 1&</th>
            <th rowspan="2">关闭指定的ConnectID的客户端连接 1 为连接ID</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint8</th><th>执行状态 0 成功</th></tr>
        <tr>
            <th rowspan="2">ForbiddenIP -c 127.0.0.2 -t 1 -s 30 &</th>
            <th rowspan="2">禁止指定的IP连接服务器，-c 是IP，-t 是类型 0 暂时 1 永久 -s 是封禁时长,单位是秒</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint8</th><th>执行状态 0 成功</th></tr>
        <tr>
            <th rowspan="7">ShowForbiddenIP -a&</th>
            <th rowspan="7">查看封禁IP列表</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint32</th><th>封禁IP个数</th></tr>
		<tr><th>uint8</th><th>封禁IP长度</th></tr>
		<tr><th>char</th><th>封禁IP</th></tr>
		<tr><th>uint8</th><th>封禁类型 0 暂时 1 永久</th></tr>
		<tr><th>uint32</th><th>封禁开始时间,格式为1970-01-01到现在的秒值</th></tr>
		<tr><th>uint32</th><th>封禁秒数</th></tr>
        <tr>
            <th rowspan="2">LiftedIP 127.0.0.2&</th>
            <th rowspan="2">解除封禁IP</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
		<tr><th>uint8</th><th>执行状态 0 成功</th></tr>
        <tr>
            <th rowspan="15">ServerConnectTCP -a&</th>
            <th rowspan="15">服务器间通讯(TCP)</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint32</th><th>服务器间连接总数</th></tr>
		<tr><th>uint8</th><th>远端服务器IP长度</th></tr>
		<tr><th>char</th><th>远端服务器IP</th></tr>
		<tr><th>uint8</th><th>连接当前状态 0已连接 1 未连接</th></tr>
		<tr><th>uint32</th><th>连接ID</th></tr>
		<tr><th>uint32</th><th>连接接收包总数</th></tr>
		<tr><th>uint32</th><th>连接发送包总数</th></tr>
		<tr><th>uint32</th><th>连接接收包总字节</th></tr>
		<tr><th>uint32</th><th>连接发送包总字节</th></tr>
		<tr><th>uint32</th><th>连接创建时间,格式为1970-01-01到现在的秒值</th></tr>
		<tr><th>uint32</th><th>连接活跃时间,单位是秒值</th></tr>
		<tr><th>uint32</th><th>连接接收队列数据包总数</th></tr>
		<tr><th>uint64</th><th>连接接收总消耗时间</th></tr>
		<tr><th>uint64</th><th>连接发送总消耗时间</th></tr>
        <tr>
            <th rowspan="15">ServerConnectUDP -a&</th>
            <th rowspan="15">服务器间通讯(UDP)</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint32</th><th>服务器间连接总数</th></tr>
		<tr><th>uint8</th><th>远端服务器IP长度</th></tr>
		<tr><th>char</th><th>远端服务器IP</th></tr>
		<tr><th>uint8</th><th>连接当前状态 0已连接 1 未连接</th></tr>
		<tr><th>uint32</th><th>连接ID</th></tr>
		<tr><th>uint32</th><th>连接接收包总数</th></tr>
		<tr><th>uint32</th><th>连接发送包总数</th></tr>
		<tr><th>uint32</th><th>连接接收包总字节</th></tr>
		<tr><th>uint32</th><th>连接发送包总字节</th></tr>
		<tr><th>uint32</th><th>连接创建时间,格式为1970-01-01到现在的秒值</th></tr>
		<tr><th>uint32</th><th>连接活跃时间,单位是秒值</th></tr>
		<tr><th>uint32</th><th>连接接收队列数据包总数</th></tr>
		<tr><th>uint64</th><th>连接接收总消耗时间</th></tr>
		<tr><th>uint64</th><th>连接发送总消耗时间</th></tr>
        <tr>
            <th rowspan="6">ShowCurrProcessInfo -a&</th>
            <th rowspan="6">查看服务器框架当前CPU,内存和网络运行状态</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint32</th><th>CPU运行比例</th></tr>
		<tr><th>uint32</th><th>当前占用内存字节数</th></tr>
		<tr><th>uint32</th><th>当前一分钟服务器网络吞吐</th></tr>
		<tr><th>uint32</th><th>当前一分钟服务器网络吞吐(入)</th></tr>
		<tr><th>uint32</th><th>当前一分钟服务器网络吞吐(出)</th></tr>
        <tr>
            <th rowspan="7">ShowAllCommand -a&</th>
            <th rowspan="7">查看所有信令运行信息</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint32</th><th>信令总数</th></tr>
		<tr><th>uint8</th><th>模块名称长度</th></tr>
		<tr><th>char</th><th>模块名称</th></tr>
		<tr><th>uint16</th><th>信令ID</th></tr>
		<tr><th>uint32</th><th>当前信令处理总数</th></tr>
		<tr><th>uint32</th><th>当前信令处理总时间,单位毫秒</th></tr>
        <tr>
            <th rowspan="2">ReConnectServer -s 172.0.10.1&</th>
            <th rowspan="2">重连远程连接 -s 远程服务器的IP地址</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint8</th><th>执行状态 0 成功</th></tr>
        <tr>
            <th rowspan="2">SetDebug -s 0 &</th>
            <th rowspan="2">设置DEBUG文件输出状态 -s 0 打开 1 关闭</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint8</th><th>执行状态 0 成功</th></tr>
        <tr>
            <th rowspan="2">ShowDebug -a&</th>
            <th rowspan="2">查询DEBUG文件输出状态 -s 0 打开 1 关闭</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint8</th><th>0 打开 1 关闭</th></tr>
        <tr>
            <th rowspan="5">GetConnectIPInfo -s 1&</th>
            <th rowspan="5">通过ConnectID获得相关的IP信息 1为ConnectID</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint16</th><th>0 找到了对应IP信息 1 没 找到了对应IP信息</th></tr>
		<tr><th>uint8</th><th>当前IP描述长度</th></tr>
		<tr><th>char</th><th>当前IP描述</th></tr>
		<tr><th>uint32</th><th>IP端口</th></tr>
        <tr>
            <th rowspan="10">GetLogInfo -a&</th>
            <th rowspan="10">得到当前日志信息</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint32</th><th>日志的个数</th></tr>
		<tr><th>uint32</th><th>当前日志等级</th></tr>
		<tr><th>uint16</th><th>日志ID</th></tr>
		<tr><th>uint8</th><th>服务器名称长度</th></tr>
		<tr><th>char</th><th>服务器名称</th></tr>
		<tr><th>uint8</th><th>日志名称长度</th></tr>
		<tr><th>char</th><th>日志名称</th></tr>
		<tr><th>uint8</th><th>日志输出类型 0 写文件 1 写屏幕</th></tr>
		<tr><th>uint16</th><th>当前日志等级</th></tr>
        <tr>
            <th rowspan="2">SetLogLevel -l 1&</th>
            <th rowspan="2">设置日志等级 -l 为 1级以上日志全部不输出</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint8</th><th>执行状态 0 成功</th></tr>
        <tr>
            <th rowspan="8">GetWorkThreadAI -a&</th>
            <th rowspan="8">得到Thread的AI配置信息</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint16</th><th>当前线程个数</th></tr>
		<tr><th>uint32</th><th>工作线程ID</th></tr>
		<tr><th>uint8</th><th>AI开关 0 关闭 1 打开</th></tr>
		<tr><th>uint32</th><th>业务包处理超时时间</th></tr>
		<tr><th>uint32</th><th>工作线程超时包的时间范围，单位是秒</th></tr>
		<tr><th>uint32</th><th>工作线程超时包的单位时间内的超时次数上限</th></tr>
		<tr><th>uint32</th><th>停止此命令服务的时间</th></tr>
        <tr>
            <th rowspan="6">GetWorkThreadTO -a&</th>
            <th rowspan="6">得到Thread的所有超时数据包信息</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint16</th><th>当前线程个数</th></tr>
		<tr><th>uint32</th><th>工作线程ID</th></tr>
		<tr><th>uint16</th><th>超时的信令ID</th></tr>
		<tr><th>uint32</th><th>超时当前时间，以1970-01-01以来开始计算的秒数</th></tr>
		<tr><th>uint32</th><th>命令执行时间，单位是毫秒</th></tr>
        <tr>
            <th rowspan="2">SetWorkThreadAI -i 1,1000,2000,10</th>
            <th rowspan="2">设置ThreadAI的配置信息,1为开启,1000处理时限 2000检查时限 10停止需要到达的次数</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint8</th><th>执行状态 0 成功</th></tr>
        <tr>
            <th rowspan="9">GetNickNameInfo -n 127.0.0.1&</th>
            <th rowspan="9">通过IP别名获得客户端IP信息</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint32</th><th>获得IP的个数</th></tr>
		<tr><th>uint32</th><th>对应的CommandID</th></tr>
		<tr><th>uint8</th><th>IP的长度</th></tr>
		<tr><th>char</th><th>IP</th></tr>
		<tr><th>uint32</th><th>IP端口</th></tr>
		<tr><th>uint8</th><th>别名长度</th></tr>
		<tr><th>char</th><th>别名</th></tr>
		<tr><th>uint8</th><th>当前连接是否日志打开 0 未开启 1 开启</th></tr>
        <tr>
            <th rowspan="2">SetConnectLog -n 1 -f 0 &</th>
            <th rowspan="2">设置连接日志开启状态 -n是 ConnectID -f是 0 开启 1 关闭</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint8</th><th>执行状态 0 成功</th></tr>
        <tr>
            <th rowspan="2">SetMaxConnectCount -n 20000 &</th>
            <th rowspan="2">设置当前服务器允许的最大连接数 -n 是连接数</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint8</th><th>执行状态 0 成功</th></tr>
        <tr>
            <th rowspan="2">AddListen -i 127.0.0.1 -p 10003 -t 1 -n 1&</th>
            <th rowspan="2">动态在服务器打开一个新的监听端口 -i IP -p 端口 -t 1 TCP 2 UDP -n 对应的PacketParseID</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint8</th><th>执行状态 0 成功</th></tr>
        <tr>
            <th rowspan="2">DelListen -i 127.0.0.1 -p 10003 -t 1 &</th>
            <th rowspan="2">动态在服务器关闭监听端口 -i IP -p 端口 -t 1 TCP 2 UDP</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint8</th><th>执行状态 0 成功</th></tr>
        <tr>
            <th rowspan="5">ShowListen -a&</th>
            <th rowspan="5">查看现有打开的服务器端口信息</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint32</th><th>监听的个数</th></tr>
		<tr><th>uint8</th><th>IP的长度</th></tr>
		<tr><th>char</th><th>IP</th></tr>
		<tr><th>uint32</th><th>IP端口</th></tr>
        <tr>
            <th rowspan="2">TestFileStart -f a.xml&</th>
            <th rowspan="2">执行文件压测指令 -f 指定压测文件名</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint8</th><th>执行状态 0 成功</th></tr>
        <tr>
            <th rowspan="2">TestFileStop -a&</th>
            <th rowspan="2">停止文件压测</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint8</th><th>执行状态 0 成功</th></tr>
        <tr>
            <th rowspan="2">SetTrackIP -i 127.0.0.1 -c 5 &</th>
            <th rowspan="2">给指定客户端IP的数据染色，-i是客户端的IP，-c是染色数据包数</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint8</th><th>执行状态 0 成功</th></tr>
        <tr>
            <th rowspan="2">SetTrackCommand -i 127.0.0.1 -c 5 &</th>
            <th rowspan="2">给指定信令ID的数据染色，-i信令ID，格式是0xXXXX，-c是染色数据包数</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint8</th><th>执行状态 0 成功，1 失败</th></tr>
        <tr>
            <th rowspan="5">GetTrackCommandInfo -a&</th>
            <th rowspan="5">查看当前染色的信令执行信息</th>
            <th>类型</th>
            <th>说明</th>
        </tr>
        <tr><th>uint8</th><th>当前信令个数</th></tr>
		<tr><th>uint16</th><th>信令ID</th></tr>
		<tr><th>uint16</th><th>信令当前已染色个数(已处理)</th></tr>
		<tr><th>uint16</th><th>信令需要染色总数</th></tr>
</table>

