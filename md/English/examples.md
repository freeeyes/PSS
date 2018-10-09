# Sample logical plug-in engineering instructions 
## (1)TcpTest plugin 
### target  
Implement standard TCP packet processing flow display。  
### Test message format  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>Test message id</th>
			<th>message description</th>
            <th colspan="2">Packet information</th>
        </tr>
        <tr>
            <th rowspan="7">COMMAND_BASE(0x1000)</th>
            <th rowspan="7">Test packet structure</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string</th><th>Packet body content</th></tr>
</table>  

## (2)UdpTest plugin 
### target  
Implement standard UDP packet processing flow display.   
### Test message format  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>Test message id</th>
			<th>message description</th>
            <th colspan="2">Packet information</th>
        </tr>
        <tr>
            <th rowspan="7">COMMAND_BASE(0x1000)</th>
            <th rowspan="7">Test packet structure</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string</th><th>Packet body content</th></tr>
</table>  

## (3)AutoTest plugin 
### target  
Realize the packet integrity detection function of PSS framework.    
### Test message format  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>Test message id</th>
			<th>message description</th>
            <th colspan="2">Packet information</th>
        </tr>
        <tr>
            <th rowspan="6">COMMAND_AUTOTEST_HEAD(0x5000)</th>
            <th rowspan="6">include header message</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>uint64</th><th>Packet timestamp</th></tr>
        <tr>
            <th rowspan="6">COMMAND_AUTOTEST_NOHEAD(0x5001)</th>
            <th rowspan="6">no include header message</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>uint64</th><th>Packet timestamp</th></tr>
        <tr>
            <th rowspan="6">COMMAND_AUTOTEST_HEADBUFF(0x5002)</th>
            <th rowspan="6">include header cache message</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>uint64</th><th>Packet timestamp</th></tr>
        <tr>
            <th rowspan="6">COMMAND_AUTOTEST_NOHEADBUFF(0x5003)</th>
            <th rowspan="6">no include header cache message</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>uint64</th><th>Packet timestamp</th></tr>
        <tr>
            <th rowspan="6">COMMAND_AUTOTEST_LOGDATA(0x5004)</th>
            <th rowspan="6">Test log</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>uint64</th><th>Packet timestamp</th></tr>
        <tr>
            <th rowspan="6">COMMAND_AUTOTEST_WORKAI(0x5006)</th>
            <th rowspan="6">Test workAI</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>uint64</th><th>Packet timestamp</th></tr>
</table>  

## (4)FtpTest plugin 
### target  
Realize simple FTP browse and download function for PSS framework.  
### Test message format  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>Test message id</th>
			<th>message description</th>
            <th colspan="2">Packet information</th>
        </tr>
        <tr>
            <th rowspan="9">COMMAND_LOGIN(0x2000)</th>
            <th rowspan="9">Login</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>uint8</th><th>Login length</th></tr>
		<tr><th>string</th><th>Login name</th></tr>
		<tr><th>uint8</th><th>Login password length</th></tr>
		<tr><th>string</th><th>Login password</th></tr>
        <tr>
            <th rowspan="7">COMMAND_LOGOUT(0x2001)</th>
            <th rowspan="7">exit</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>uint8</th><th>Login length</th></tr>
		<tr><th>string</th><th>Login name</th></tr>
        <tr>
            <th rowspan="9">COMMAND_FILELIST(0x2002)</th>
            <th rowspan="9">file and path list</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>uint8</th><th>Login name length</th></tr>
		<tr><th>string</th><th>Login name</th></tr>
		<tr><th>uint8</th><th>Current path length</th></tr>
		<tr><th>string</th><th>Current path</th></tr>
        <tr>
            <th rowspan="11">COMMAND_FILE_DOWNLOAD(0x2003)</th>
            <th rowspan="11">download file</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>uint8</th><th>Login name length</th></tr>
		<tr><th>string</th><th>Login name</th></tr>
		<tr><th>uint8</th><th>Current file name length</th></tr>
		<tr><th>string</th><th>Current file name</th></tr>
		<tr><th>uint32</th><th>paacket length</th></tr>
		<tr><th>uint32</th><th>Packet sequence number, first 0</th></tr>
        <tr>
            <th rowspan="14">COMMAND_FILE_UPLOAD(0x2004)</th>
            <th rowspan="14">upload file</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>uint64</th><th>Packet timestamp</th></tr>
		<tr><th>uint8</th><th>Login name length</th></tr>
		<tr><th>string</th><th>Login name</th></tr>
		<tr><th>uint8</th><th>Current file name length</th></tr>
		<tr><th>string</th><th>Current file name</th></tr>
		<tr><th>uint32</th><th>paacket length</th></tr>
		<tr><th>uint32</th><th>Packet sequence number, first 0</th></tr>
		<tr><th>uint32</th><th>File block size</th></tr>
		<tr><th>string</th><th>File block</th></tr>
</table>  

## (5)Http plugin 
### target  
Implement http command parsing support for PSS.  
### Test message format  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>Test message id</th>
			<th>message description</th>
            <th colspan="2">Packet information</th>
        </tr>
        <tr>
            <th rowspan="6">COMMAND_HTTP_DATA(0xea01)</th>
            <th rowspan="6">Http data request protocol packet</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>string</th><th>HttpPacket body content</th></tr>
</table>  

## (6)kafka plugin 
### target  
Implementation of PSS framework for Kafka interface support.    
### Test message format  
No instructions received, this function is to demonstrate the integration of Kafka 's consumer thread and PSS, Kafka 's consumer information received and issued.  

## (7)LoginBuffer plugin 
### target  
Realize the interaction between PSS and shared memory, and realize the reading and writing of users through shared memory.    
### Test message format  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>Test message id</th>
			<th>message description</th>
            <th colspan="2">Packet information</th>
        </tr>
        <tr>
            <th rowspan="9">COMMAND_LOGIN(0x2100)</th>
            <th rowspan="9">Login</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>uint8</th><th>Login name length</th></tr>
		<tr><th>string</th><th>Login name</th></tr>
		<tr><th>uint8</th><th>password length</th></tr>
		<tr><th>string</th><th>password</th></tr>
        <tr>
            <th rowspan="7">COMMAND_LOGOUT(0x2101)</th>
            <th rowspan="7">exit</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>uint8</th><th>Login name length</th></tr>
		<tr><th>string</th><th>Login name</th></tr>
        <tr>
            <th rowspan="6">COMMAND_USERINFO(0x2102)</th>
            <th rowspan="6">Search user</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>uint32</th><th>Login nameID</th></tr>
        <tr>
            <th rowspan="8">COMMAND_SET_USERINFO(0x2103)</th>
            <th rowspan="8">set user</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>uint32</th><th>Login nameID</th></tr>
		<tr><th>uint32</th><th>user life</th></tr>
		<tr><th>uint32</th><th>user magic</th></tr>
</table> 

## (8)LoginBuffer plugin 
### target  
Realize the interaction between PSS and shared memory, and realize the reading and writing of users through shared memory.    
### Test message format  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>Test message id</th>
			<th>message description</th>
            <th colspan="2">Packet information</th>
        </tr>
        <tr>
            <th rowspan="9">COMMAND_LOGIN(0x2100)</th>
            <th rowspan="9">Login</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>uint8</th><th>Login name length</th></tr>
		<tr><th>string</th><th>Login name</th></tr>
		<tr><th>uint8</th><th>Password length</th></tr>
		<tr><th>string</th><th>Password</th></tr>
        <tr>
            <th rowspan="7">COMMAND_LOGOUT(0x2101)</th>
            <th rowspan="7">exit</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>uint8</th><th>Login name length</th></tr>
		<tr><th>string</th><th>Login name</th></tr>
        <tr>
            <th rowspan="6">COMMAND_USERINFO(0x2102)</th>
            <th rowspan="6">Search user</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>uint32</th><th>Login nameID</th></tr>
        <tr>
            <th rowspan="8">COMMAND_SET_USERINFO(0x2103)</th>
            <th rowspan="8">Set user</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>uint32</th><th>Login nameID</th></tr>
		<tr><th>uint32</th><th>user life</th></tr>
		<tr><th>uint32</th><th>user magic</th></tr>
</table> 

## (9)LuaTest plugin 
### target  
Implement call to PSS and Lua scripts.    
### Test message format  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>Test message id</th>
			<th>message description</th>
            <th colspan="2">Packet information</th>
        </tr>
        <tr>
            <th rowspan="6">COMMAND_BASE(0x1000)</th>
            <th rowspan="6">Login</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>string</th><th>params</th></tr>
</table> 

## (10)PlugAtoB plugin 
### target  
Implement call to PSS plug-ins.   
### Test message format  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>Test message id</th>
			<th>message description</th>
            <th colspan="2">Packet information</th>
        </tr>
        <tr>
            <th rowspan="6">COMMAND_PLUGA(0x2201)</th>
            <th rowspan="6">Implement inter-plug-in message notification</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>uint8</th><th>data length</th></tr>
		<tr><th>string</th><th>data</th></tr>
</table>  

## (11)Proxy plugin 
### target  
Message forwarding between client and server of PSS is realized.  
### Test message format  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>Test message id</th>
			<th>message description</th>
            <th colspan="2">Packet information</th>
        </tr>
        <tr>
            <th rowspan="6">COMMAND_PROXY(0x1030)</th>
            <th rowspan="6">Messages that need to be forwarded to other servers</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>string</th><th>Data params</th></tr>
</table>  

## (12)TestTCPPost plugin 
### target  
Tcp data transmission to PSS is realized.  
### Test message format  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>Test message id</th>
			<th>message description</th>
            <th colspan="2">Packet information</th>
        </tr>
        <tr>
            <th rowspan="6">COMMAND_PROXY(0x1030)</th>
            <th rowspan="6">Messages that need to be forwarded to other servers</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>string</th><th>Data params</th></tr>
</table>  

## (13)TestUDPPost plugin 
### target  
Realize UDP data transmission to PSS.  
### Test message format  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>Test message id</th>
			<th>message description</th>
            <th colspan="2">Packet information</th>
        </tr>
        <tr>
            <th rowspan="6">COMMAND_PROXY(0x1030)</th>
            <th rowspan="6">Messages that need to be forwarded to other servers</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>string</th><th>Data params</th></tr>
</table>  

## (14)TimerTest plugin 
### target  
Implements a timer call plug-in to PSS.  
### Test message format  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>Test message id</th>
			<th>message description</th>
            <th colspan="2">Packet information</th>
        </tr>
        <tr>
            <th rowspan="6">COMMAND_TIMER(0x1100)</th>
            <th rowspan="6">Timer call instruction</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>string</th><th>Data params</th></tr>
</table>  


## (15)WebSocket plugin 
### target  
Realize the WebSocket message parsing of PSS.  
### Test message format  
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th>Test message id</th>
			<th>message description</th>
            <th colspan="2">Packet information</th>
        </tr>
        <tr>
            <th rowspan="6">COMMAND_HANDIN(0xee01)</th>
            <th rowspan="6">websocket message header validation</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>string</th><th>Data params</th></tr>
        <tr>
            <th>Test message id</th>
			<th>message description</th>
            <th colspan="2">Packet information</th>
        </tr>
        <tr>
            <th rowspan="6">COMMAND_DATAIN(0xee02)</th>
            <th rowspan="6">websocket message body</th>
            <th>Attribute</th>
            <th>description</th>
        </tr>
		<tr><th>uint16</th><th>Packet version number</th></tr>
		<tr><th>uint16</th><th>Packet command id</th></tr>
		<tr><th>uint32</th><th>Packet length</th></tr>
		<tr><th>string(32 chars)</th><th>Packet session</th></tr>
		<tr><th>string</th><th>Data params</th></tr>
</table>  

## (15)ZooClient plugin 
### target  
Implements the call to the zookeeper message by PSS.  
### Test message format  
No packets, just realize the discovery function of zookeeper.    