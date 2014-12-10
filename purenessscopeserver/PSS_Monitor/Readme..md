##此工具设计目的

此工具是PSS的跨平台客户端工具组之一，这个工具可以监控指定多台的PSS主机的运行情况，包括：
* 活跃连接数ActiveClient，当前PSS服务中已经连接的客户端数量。
* 连接池数量PoolClient，当前PSS服务中的连接池的大小。
* 最大允许连接数量MaxHandlerCount，当前PSS允许的最大连接数量
* FlowIn，当前PSS服务接收的客户端流量单位总和（1分钟刷新一次）
* FlowOut 当前PSS服务发送的客户端流量单位总和（1分钟刷新一次）
   
通过配置配置文件可以达成以上目标(Main.xml)。
你可以配置你的接收邮箱。
比如：  
* * * 
<code>
\<Mail MailID="0" fromMailAddr="local@163.com" toMailAddr="aaaa@163.com" MailPass="123456" MailUrl="smtp.163.com" MailPort="25" />
</code>
* * * 

并可以通过增加指定的XML配置，实现同时监控多台PSS的目标。
* * *
<code>
\<TCPServerIP name="PSS1" ip="127.0.0.1" port="10010" ipType="IPV4" key="freeeyes" />  
\<TCPServerIP name="PSS1" ip="127.0.0.2" port="10010" ipType="IPV4" key="freeeyes1" />
</code>
* * *

这样就实现了，对指定的PSS服务运行情况进行监控，并将监控结果生成为XML页面。这个XML页面可以通过http服务直接展示成界面。  
具体XML位置为MonitorLog/服务器name/name_date.xml