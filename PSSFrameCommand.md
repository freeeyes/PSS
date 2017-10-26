#PSS框架命令指令集
##1.命令组成
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

##2.命令说明（以二进制结果输出返回）
<table>
        <tr>
            <th>命令样例</th>
			<th>说明</th>
            <th>返回数据结构</th>
        </tr>
        <tr>
            <th>ShowServerInfo -a</th>
            <th>获得框架基本运行参数</th>
            <th>
				<table>
				<tr><th>类型</th><th>说明</th></tr>
				<th>uint16</th><th>服务器ID</th></tr>
				<th>uint8</th><th>服务器名称字符串长度</th></tr>
				<th>char</th><th>服务器名称</th></tr>
				<th>uint8</th><th>服务版本字符串长度</th></tr>
				<th>char</th><th>服务器版本</th></tr>
				<th>uint16</th><th>服务器插件个数</th></tr>
				<th>uint16</th><th>服务器工作线程个数</th></tr>
				<th>uint16</th><th>服务器协议包版本号</th></tr>
				<th>uint8</th><th>服务器字序大端还是小段，0小端 1大端</th></tr>
				<th>uint8</th><th>服务器字序 0 主机序 1 网络序</th></tr>
				</table>
			</th>
        </tr>
</table>
