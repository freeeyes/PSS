# PSS framework profile
> PSS all the configuration files are in XML format, including all the parameters of the running framework.
> you can modify these parameters according to your needs, to achieve the purpose of the control framework.
> currently supported is，main.xml,logger.xml,forbiddenIP.xml,alert.xml,FileTestCfg.xml,ConsoleCommand.xml。  

## mian.xml(Main frame configuration information)  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>Label</th>
			<th>Label description</th>
            <th colspan="2">Attribute information</th>
        </tr>
        <tr>
            <th rowspan="5">NetWorkMode</th>
            <th rowspan="5">Framework network mode configuration</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>Mode</th><th>Network mode, currently supported for windows is IOCP,Select;Linux下为Epoll,Select,poll,Epoll_et</th></tr>
		<tr><th>BackLog</th><th>socket length of receive wait queue</th></tr>
		<tr><th>ByteOrder</th><th>Server word order, buffpacket according to this word order to resolve the number in the packet, the parameter is HOST_ORDER或NET_ORDER</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="8">ServerType</th>
            <th rowspan="8">Basic service information</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>Type</th><th>Set 0 to run in foreground and 1 to run in background ( 1 to start in service mode under windows )</th></tr>
		<tr><th>name</th><th>Name of the current service ( used in windows service state only )</th></tr>
		<tr><th>displayname</th><th>Display name of the current server ( used in windows service state only )</th></tr>
		<tr><th>Debug</th><th>Whether debug mode is turned on, 0 bits off, 1 is turned on, and the framework will output additional data to run the log after turning on</th></tr>
		<tr><th>DebugSize</th><th>If debug is on, the maximum output length for a single log is set here</th></tr>
		<tr><th>IsClose</th><th>Remote shutdown service is supported, 0 is not supported, 1 is supported</th></tr>	
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="3">ServerID</th>
            <th rowspan="3">Server ID</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>id</th><th>Server ID</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="3">ServerName</th>
            <th rowspan="3">Server Name</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>name</th><th>Server Name</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="3">ServerVersion</th>
            <th rowspan="3">Server Version</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>Version</th><th>Server Version</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="5">TCPServerIP</th>
            <th rowspan="5">Server listening IP ( can be multiple )</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>ip</th><th>Listen for IP, default is INADDR_ANY</th></tr>
		<tr><th>port</th><th>Listen port</th></tr>
		<tr><th>ipType</th><th>Ip address mode, IPv4 and IPv6 support</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="7">PacketParse</th>
            <th rowspan="7">Packet resolution plug-in location ( can be multiple )</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>ParseID</th><th>Parse ID</th></tr>
		<tr><th>ModulePath</th><th>Path to plug - in</th></tr>
		<tr><th>ModuleName</th><th>File name of the plug - in</th></tr>
		<tr><th>Type</th><th>the type of plug - in H&B or STREAM</th></tr>
		<tr><th>OrgLength</th><th>If H&B refers to the packet header length, and if stream mode refers to the maximum packet length</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="5">Console</th>
            <th rowspan="5">Frame control instruction receiving port</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>support</th><th>0 is an open port and 1 is an open port</th></tr>
		<tr><th>sip</th><th>Monitor IP address, default is INADDR_ANY</th></tr>
		<tr><th>sport</th><th>Monitor port</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="8">Message</th>
            <th rowspan="8">Message worker thread control related parameters</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>Msg_High_mark</th><th>High water mark for worker thread queue</th></tr>
		<tr><th>Msg_Low_mark</th><th>Low water mark for worker thread queue</th></tr>
		<tr><th>Msg_Buff_Max_Size</th><th>Maximum number of bytes held by work thread queue pending commands</th></tr>
		<tr><th>Msg_Process</th><th>Process mode runs worker threads, and the number represents the number of worker processes started</th></tr>
		<tr><th>Msg_Thread</th><th>Number of worker threads started</th></tr>
		<tr><th>Msg_MaxQueue</th><th>Maximum number of unprocessed messages allowed for worker thread message queues</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="3">ModuleManager</th>
            <th rowspan="3">Maximum number of logical plug-in modules</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>MaxCount</th><th>Maximum number of logical plug-in modules</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="5">ModuleInfo</th>
            <th rowspan="5">Logical plug-in module information ( can be multiple )</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>ModuleSPath</th><th>Logical plug - in module path</th></tr>
		<tr><th>ModuleSName</th><th>Logical plug - in module name</th></tr>
		<tr><th>ModuleSParam</th><th>Logical plug - in startup parameters</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="4">CoreSetting</th>
            <th rowspan="4">Coredump file generation configuration ( Linux only )</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>CoreNeed</th><th>0 is off and 1 is on</th></tr>
		<tr><th>Script</th><th>When a program sends a crash, if a script path and file are set here, the script is automatically called for execution.</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="5">CommandAccount</th>
            <th rowspan="5">Message command access statistics logging</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>Account</th><th>Packets are counted by number of signaling, 0 is off, 1 is on</th></tr>
		<tr><th>FlowAccount</th><th>Packets are counted by signaling traffic, 0 is off, 1 is on</th></tr>
		<tr><th>MaxCommandCount</th><th>Maximum number of message command lists supported</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="4">RecvInfo</th>
            <th rowspan="4">Receive packet configuration correlation</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>RecvBuffSize</th><th>Sets the socket receive buffer size</th></tr>
		<tr><th>RecvQueueTimeout</th><th>Set receive socket timeout</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="10">SendInfo</th>
            <th rowspan="10">Send data queue configuration related</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>SendQueueMax</th><th>Maximum number of data sent by the send data queue</th></tr>
		<tr><th>TcpNodelay</th><th>Sets the nodelay algorithm parameters for socket</th></tr>
		<tr><th>MaxBlockSize</th><th>Maximum packet length sent</th></tr>
		<tr><th>BlockCount</th><th>Maximum number of packets in the send buffer pool that can be buffered</th></tr>
		<tr><th>SendTimeout</th><th>Socket send timeout ( in milliseconds )</th></tr>
		<tr><th>SendQueueTimeout</th><th>Maximum write time to send queue</th></tr>
		<tr><th>SendQueueCount</th><th>Number of transmit queues</th></tr>
		<tr><th>PutQueueTimeout</th><th>Timeout period for sending packets into the sending queue</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="6">ThreadInfo</th>
            <th rowspan="6">Worker thread self-test configuration</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>ThreadTimeout</th><th>Worker thread self-test cycle, in seconds</th></tr>
		<tr><th>ThreadTimeCheck</th><th>The maximum time, in seconds, that a worker thread determines whether to execute a deadlock or not respond</th></tr>
		<tr><th>DisposeTimeout</th><th>Worker thread processing timeout alarm</th></tr>
		<tr><th>PutQueueTimeout</th><th>Maximum time-out for data to be placed in a worker thread</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="8">ThreadInfoAI</th>
            <th rowspan="8">The worker thread filters the timeout signaling configuration according to the signaling processing time</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>AI</th><th>0 is off and 1 is on</th></tr>
		<tr><th>CheckTime</th><th>Packet processing time detection period, in seconds</th></tr>
		<tr><th>TimeoutCount</th><th>Packet processing timeout, in seconds</th></tr>
		<tr><th>StopTime</th><th>When a packet continuously times out, the time, in seconds, to stop receiving a signaling packet</th></tr>
		<tr><th>ReturnDataType</th><th>Returns the instruction type, 1 - bit binary, 0 text, that does not process the packet to the client when the packet continues to process the timeout</th></tr>
		<tr><th>ReturnData</th><th>Returns a binary text information packet body</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="7">ClientInfo</th>
            <th rowspan="7">Client access connection management configuration</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>HandlerCount</th><th>Maximum number of data connections allowed current pool size</th></tr>
		<tr><th>MaxHandlerCount</th><th>Maximum number of maximum buffer pool sizes allowed for data connections</th></tr>
		<tr><th>MaxConnectTime</th><th>When data link connection, more than this time does not send data, will automatically disconnect, in seconds</th></tr>
		<tr><th>CheckAliveTime</th><th>The period of time, in seconds, that the data state of the data link is detected</th></tr>
		<tr><th>MaxBuffRecv</th><th>Maximum size of received packet</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="6">ConnectServer</th>
            <th rowspan="6">Configuration of connection information between servers</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>TimeInterval</th><th>Maximum timeout, in seconds, for connection alive detection between servers</th></tr>
		<tr><th>TimeCheck</th><th>Self - test cycle time, in seconds</th></tr>
		<tr><th>Recvbuff</th><th>Maximum size of packets received between servers</th></tr>
		<tr><th>RunType</th><th>Returns packet processing mode between servers, 0 is synchronous and 1 is asynchronous</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="5">Monitor</th>
            <th rowspan="5">Pss framework monitoring information</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>CpuAndMemory</th><th>Cpu and memory monitoring, 0 is off, 1 is on</th></tr>
		<tr><th>CpuMax</th><th>Maximum CPU alarm limit</th></tr>
		<tr><th>MemoryMax</th><th>Maximum memory usage alarm limit</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="8">AceDebug</th>
            <th rowspan="8">Acedebug setting output</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>TrunOn</th><th>0 is off and 1 is on</th></tr>
		<tr><th>DebugName</th><th>Debug information output file name</th></tr>
		<tr><th>ChkInterval</th><th>Detection period</th></tr>
		<tr><th>LogFileMaxSize</th><th>Maximum size of a single log file</th></tr>
		<tr><th>LogFileMaxCnt</th><th>Number of log file loop overrides</th></tr>
		<tr><th>Level</th><th>Log output level</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="2">TSTimer</th>
            <th rowspan="2">TSTimer configure</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>TimerListPool</th><th>The maximum number of tasks allowed for the current timer pool. if it is 0, the ts timer thread will not be started by default.</th></tr>
        <tr>
            <th rowspan="6">TcpRedirection</th>
            <th rowspan="6">Data forwarding</th>
            <th>Attribute</th>
            <th>information</th>
            <tr><th>SrcPort</th><th>Local ports that need corresponding forwarding</th></tr>
		    <tr><th>RedirectionIP</th><th>Forward IP</th></tr>
			<tr><th>RedirectionPort</th><th>Forward Port</th></tr>
			<tr><th>Mode</th><th>0 only forwards data and does not respond to the client with response data. 1 Forward data and accept the response data from the forwarding server to the client</th></tr>
            <tr><th>ConnectState</th><th>0 is out of sync with the forwarding connection. 1 is synchronized with the forwarding connection</th></tr>
        </tr>
        <tr>
            <th rowspan="6">ServerToServer</th>
            <th rowspan="6">Connection information configuration between servers</th>
            <th>Attribute</th>
            <th>information</th>
            <tr><th>S2SID</th><th>Remote server ID, must be unique</th></tr>
		    <tr><th>S2SIp</th><th>Remote server IP</th></tr>
			<tr><th>S2SPort</th><th>Remote server port</th></tr>
			<tr><th>S2SPacketParseID</th><th>The ID of the corresponding message parsing plugin PacketParse</th></tr>
        </tr>

</table>

## logger.xml(Framework log configuration information)  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>Label</th>
			<th>Label information</th>
            <th colspan="2">Attribute information</th>
        </tr>
        <tr>
            <th rowspan="4">ServerLogHead</th>
            <th rowspan="4">Server log file header</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>Text</th><th>Server log file header</th></tr>
		<tr><th>LogFileMaxSize</th><th>Maximum size of a single log block in mb</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="3">LogPath</th>
            <th rowspan="3">Log folder path</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>Path</th><th>Log folder path</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="4">LogPool</th>
            <th rowspan="4">Logging pool related configuration</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>BlockSize</th><th>Single log block size</th></tr>
		<tr><th>PoolCount</th><th>Number of log buffer pools</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="3">LogLevel</th>
            <th rowspan="3">Log level related configuration</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>CurrLevel</th><th>The log level, only more than this level of the log will be recorded, below this level of the log will not be recorded</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="7">LogInfo</th>
            <th rowspan="7">Specific log information ( can be multiple )</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>logid</th><th>Id of the current log, must be unique</th></tr>
		<tr><th>logtype</th><th>Log type, 1 is the operation folder log, 2 is the servererror log</th></tr>
		<tr><th>Display</th><th>Whether to display output, if it is selected to display, no longer output to the file, 0 is no, 1 is yes</th></tr>
		<tr><th>Level</th><th>The current log level, which can be set as desired, corresponds to currlevel</th></tr>
		<tr><th>logname</th><th>Log name, which is generated one day</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
</table>
## forbiddenIP.xml(Ip disable configuration information)  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>Label</th>
			<th>Labelinformation</th>
            <th colspan="2">Attribute information</th>
        </tr>
        <tr>
            <th rowspan="4">ForbiddenIP</th>
            <th rowspan="4">List of client IPS that are not accessible ( can be multiple )</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>ip</th><th>Client IP to disable connection</th></tr>
		<tr><th>type</th><th>Connect type,TCP or UDP</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
</table>
## alert.xml(Framework alert configuration information)  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>Label</th>
			<th>Labelinformation</th>
            <th colspan="2">Attribute information</th>
        </tr>
        <tr>
            <th rowspan="7">AlertConnect</th>
            <th rowspan="7">Connection information alert configuration</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>ConnectMin</th><th>The current minimum number of connections ( within 1 minute ), if 0, is not monitored and the message is alerted if this number is exceeded</th></tr>
		<tr><th>ConnectMax</th><th>The current maximum number of connections ( within 1 minute ), if 0, is not monitored and the message is alerted if this number is exceeded</th></tr>
		<tr><th>DisConnectMin</th><th>The current minimum number of disconnected connections ( within 1 minute ) is not monitored if 0, and alert message</th></tr>
		<tr><th>DisConnectMax</th><th>The current maximum number of disconnected connections ( within 1 minute ), if 0, is not monitored and the message is alerted if this number is exceeded</th></tr>
		<tr><th>ConnectAlert</th><th>The maximum number of connections currently in existence, exceeding which alert messages</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="4">IP</th>
            <th rowspan="4">Client IP connections monitoring alerts</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>IPMax</th><th>The maximum number of IP connections ( in units of time ) for the same client, exceeding which messages are alerted</th></tr>
		<tr><th>Timeout</th><th>Detection period, unit time is long, unit is seconds</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="7">ClientData</th>
            <th rowspan="7">Per unit time, a single connection receives the transmit data monitoring configuration</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>RecvPacketCount</th><th>Maximum number of packets received for a single connection in 1 minute, exceeding this number, alerts the message</th></tr>
		<tr><th>RecvDataMax</th><th>The maximum total packet size threshold for receiving a single connection within 1 minute, exceeding which the message is alerted</th></tr>
		<tr><th>SendPacketCount</th>Maximum number of packets sent for a single connection in 1 minute, exceeding this number, alerts the message</th></tr>
		<tr><th>SendDataMax</th><th>The maximum total packet size threshold for sending a single connection within 1 minute, exceeding which an alert message is sent</th></tr>
		<tr><th>MailID</th><th>Id corresponding to the message sent</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="5">CommandInfo</th>
            <th rowspan="5">Signaling monitoring configuration information ( can be multiple )</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>CommandID</th><th>Signaling id to monitor</th></tr>
		<tr><th>CommandCount</th><th>The maximum number of minutes to process the signaling, 0 is not monitored, more than this number will alert the message</th></tr>
		<tr><th>MailID</th><th>Id corresponding to the message sent</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="8">Mail</th>
            <th rowspan="8">Alert mail related configuration ( can be multiple )</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>MailID</th><th>Alert mail id</th></tr>
		<tr><th>fromMailAddr</th><th>Set up the sender of the mail</th></tr>
		<tr><th>toMailAddr</th><th>Set up message recipients</th></tr>
		<tr><th>MailPass</th><th>Mailbox password</th></tr>
		<tr><th>MailUrl</th><th>Mailbox SMTP address</th></tr>
		<tr><th>MailPort</th><th>Smtp port for mailbox</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
</table>
## ConsoleCommand.xml(Framework control instruction permission configuration information)  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>Label</th>
			<th>Labelinformation</th>
            <th colspan="2">Attribute information</th>
        </tr>
        <tr>
            <th rowspan="4">CommandInfo</th>
            <th rowspan="4">Background command permission configuration for PSS framework ( can be multiple )</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>CommandName</th><th>Command name</th></tr>
		<tr><th>User</th><th>Allowed user information, multiple users separated by commas</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
</table>
## FileTestCfg.xml(File test interface configuration information)  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>Label</th>
			<th>Labelinformation</th>
            <th colspan="2">Attribute information</th>
        </tr>
        <tr>
            <th rowspan="10">FileTestConfig</th>
            <th rowspan="10">Test file interface entry packet configuration</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>Path</th><th>Test file packet path</th></tr>
		<tr><th>TimeInterval</th><th>Test time period, in seconds</th></tr>
		<tr><th>ConnectCount</th><th>Number of unit cycle connections established</th></tr>
		<tr><th>ResponseCount</th><th>Number of unit cycle packet processing returns</th></tr>
		<tr><th>ExpectTime</th><th>The total time spent looking forward to testing, in milliseconds</th></tr>
		<tr><th>ParseID</th><th>Parser plug - in id that needs to work with the framework</th></tr>
		<tr><th>ContentType</th><th>Connection type, 1 is a binary packet, 0 is a text packet</th></tr>
		<tr><th>ContentType</th><th>Connection type, 1 is TCP,2 is UDP</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
        <tr>
            <th rowspan="3">FileInfo</th>
            <th rowspan="3">Test package configuration ( can be multiple )</th>
            <th>Attribute</th>
            <th>information</th>
        </tr>
		<tr><th>FileName</th><th>Test packet file name</th></tr>
		<tr><th>desc</th><th>Current label description information</th></tr>
</table>