# Pss framework console command group   
## 1.command struct  
The PSS framework supports instruction sets that operate on the internal health of the framework.    
These instructions include framework plug-in management, worker thread state management, framework basic parameter query, connection number list, server port management, IP management, and so on.  

Command format:  
<Command output type> <user name> <command name> <command params><Command terminator>  
ezample:  
b freeeyes ShowServerInfo -a&  
**b**: The output is output as a binary result, and if you want to output as a text structure, you can use **( for example, under telnet )  
**freeeyes**: Username, this is required, the PSS framework will verify the user, specific verification settings in **main.xml **(set username) and **consolecommand.xml **( set corresponding permissions ) have permissions to be executed.    
**ShowServerInfo**: Command name.  
**-a**: Command params.  
**&**: Command terminator is required.

##2.Support command introduction (Returns as a binary result output)   
<table width="100%" border="1" cellpadding="0" cellspacing="0">
        <tr>
            <th width="20%">Command sample</th>
			<th width="20%">Explain</th>
            <th width="60%" colspan="2">Returns the data structure</th>
        </tr>
        <tr>
            <th rowspan="12">ShowServerInfo -a</th>
            <th rowspan="12">Obtain the basic operating parameters of the framework</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
		<tr><th>uint16</th><th>Server ID</th></tr>
		<tr><th>uint8</th><th>Server name length</th></tr>
		<tr><th>char</th><th>Server name</th></tr>
		<tr><th>uint8</th><th>Server version code length</th></tr>
		<tr><th>char</th><th>Server version code</th></tr>
		<tr><th>uint16</th><th>Server logic plugin count</th></tr>
		<tr><th>uint16</th><th>Server work thread count</th></tr>
		<tr><th>uint16</th><th>Server packet version</th></tr>
		<tr><th>uint8</th><th>Server word order large or small, 0 small 1 large</th></tr>
		<tr><th>uint8</th><th>Server word sequence 0 host sequence 1 network sequence</th></tr>
        <tr>
            <th rowspan="17">ShowModule -a</th>
            <th rowspan="17">View all the plug-ins that the framework is running</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
		<tr><th>uint32</th><th>Number of plug-ins currently running</th></tr>
		<tr><th>uint8</th><th>Plug - in filename length</th></tr>
		<tr><th>char</th><th>Plug - in file name</th></tr>
		<tr><th>uint8</th><th>Plug - in name length</th></tr>
		<tr><th>char</th><th>Plug - in name</th></tr>
		<tr><th>uint8</th><th>Plug - in path length</th></tr>
		<tr><th>char</th><th>Plug - in path</th></tr>
		<tr><th>uint8</th><th>Plug - in startup parameter length</th></tr>
		<tr><th>char</th><th>Plug - in startup parameter</th></tr>
		<tr><th>uint8</th><th>Plug - in description length</th></tr>
		<tr><th>char</th><th>Plug - in description</th></tr>
		<tr><th>uint8</th><th>Plug - in load time length</th></tr>
		<tr><th>char</th><th>Plug - in load time</th></tr>
		<tr><th>uint8</th><th>Plug - in current running status word 0 is ok</th></tr>
		<tr><th>uint32</th><th>Plug - in currently running error code 0 is ok</th></tr>
        <tr>
            <th rowspan="3">LoadModule ./,TcpTest.so,Param</th>
            <th rowspan="3">Load a plug - in, tcptest.so is the file name,. / is the path of the plug-in on the server, param is the plug-in startup parameter</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint8</th><th>Plugin startup result 0 succeeded 1 failed</th></tr>
        <tr>
            <th rowspan="3">UnLoadModule TestTcp</th>
            <th rowspan="3">Uninstall a plug - in, testtcp is the plug-in 's own name ( note that it is not a file name, it is a name set inside the plug - in )</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint8</th><th>Plugin startup result 0 succeeded 1 failed</th></tr>
        <tr>
            <th rowspan="3">ReLoadModule TestTcp</th>
            <th rowspan="3">Reload a plug - in, testtcp is the plug-in 's own name ( note that it is not a file name, it is a name set inside the plug - in )</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint8</th><th>Plugin startup result 0 succeeded 1 failed</th></tr>
        <tr>
            <th rowspan="5">ClientCount -cp</th>
            <th rowspan="5">Current number of client connections</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
		<tr><th>uint32</th><th>Total number of current client connections</th></tr>
		<tr><th>uint32</th><th>Current total number of client connection pool remaining</th></tr>
		<tr><th>uint32</th><th>Total number of current client connection pools</th></tr>
        <tr>
            <th rowspan="7">CommandInfo 0x1000</th>
            <th rowspan="7">Query the specified message information, 0x1000 for message hexadecimal id</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
		<tr><th>uint32</th><th>Number of message</th></tr>
		<tr><th>uint16</th><th>Current number of message</th></tr>
		<tr><th>uint16</th><th>Message id</th></tr>
		<tr><th>uint32</th><th>Message execute counts</th></tr>
		<tr><th>uint64</th><th>Total time that Message has been performed,In seconds</th></tr>
        <tr>
            <th rowspan="6">CommandTimeout -a</th>
            <th rowspan="6">Query all timeout message lists</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
		<tr><th>uint32</th><th>Timeout message count</th></tr>
		<tr><th>uint16</th><th>Timeout message ID</th></tr>
		<tr><th>uint32</th><th>Timeout message occurrence</th></tr>
		<tr><th>uint32</th><th>Timeout message run times</th></tr>
        <tr>
            <th rowspan="3">CommandTimeoutclr -a</th>
            <th rowspan="3">Clear the timeout message list</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint8</th><th>Execution state 0 succeeded</th></tr>
        <tr>
            <th rowspan="3">CommandDataLog -a</th>
            <th rowspan="3">save CommandDataLog</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint8</th><th>Execution state 0 succeeded</th></tr>
        <tr>
            <th rowspan="11">WorkThreadState -s</th>
            <th rowspan="11">View worker thread status</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
		<tr><th>uint8</th><th>Work thread count</th></tr>
		<tr><th>uint8</th><th>Work thread ID</th></tr>
		<tr><th>uint32</th><th>The last run time of the worker thread, in the format 1970-01-01 to the current second value</th></tr>
		<tr><th>uint32</th><th>Worker thread start time, in the format 1970-01-01 to the current seconds</th></tr>
		<tr><th>uint8</th><th>Current state of worker thread</th></tr>
		<tr><th>uint32</th><th>工Total number packets currently processed by work thread</th></tr>
		<tr><th>uint16</th><th>The message id that that worker thread is currently run</th></tr>
		<tr><th>uint16</th><th>Average packet processing time, in milliseconds, for worker threads</th></tr>
		<tr><th>uint16</th><th>Number of packets processed by worker threads in the last minute</th></tr>
        <tr>
            <th rowspan="15">ConnectClient -a</th>
            <th rowspan="15">Information for the current TCP client link</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint32</th><th>Total number of current connections</th></tr>
		<tr><th>uint8</th><th>Connection name string length</th></tr>
		<tr><th>char</th><th>Connection name string, the default is the connection of the client IP, can also be set</th></tr>
		<tr><th>uint32</th><th>Connect ID</th></tr>
		<tr><th>uint32</th><th>Connect recv packet counts</th></tr>
		<tr><th>uint32</th><th>Connect send packet counts</th></tr>
		<tr><th>uint32</th><th>Connect recv packet sizes</th></tr>
		<tr><th>uint32</th><th>Connect send packet sizes</th></tr>
		<tr><th>uint32</th><th>Connection creation time, in the format 1970-01-01 to the current second value</th></tr>
		<tr><th>uint32</th><th>Connection active time in seconds</th></tr>
		<tr><th>uint32</th><th>Total number of connection receive queue packets</th></tr>
		<tr><th>uint64</th><th>Total connection reception elapsed time</th></tr>
		<tr><th>uint64</th><th>Total elapsed time for connection transmission</th></tr>
        <tr>
            <th rowspan="15">UDPConnectClient -a</th>
            <th rowspan="15">Information for the current UDP client link</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint32</th><th>Total number of current connections</th></tr>
		<tr><th>uint8</th><th>Connection name string length</th></tr>
		<tr><th>char</th><th>Connection name string, the default is the connection of the client IP, can also be set</th></tr>
		<tr><th>uint32</th><th>Connect ID</th></tr>
		<tr><th>uint32</th><th>Connect recv packet counts</th></tr>
		<tr><th>uint32</th><th>Connect send packet counts</th></tr>
		<tr><th>uint32</th><th>Connect recv packet sizes</th></tr>
		<tr><th>uint32</th><th>Connect send packet sizes</th></tr>
		<tr><th>uint32</th><th>Connection creation time, in the format 1970-01-01 to the current second value</th></tr>
		<tr><th>uint32</th><th>Connection active time in seconds</th></tr>
		<tr><th>uint32</th><th>Total number of connection receive queue packets</th></tr>
		<tr><th>uint64</th><th>Total connection reception elapsed time</th></tr>
		<tr><th>uint64</th><th>Total elapsed time for connection transmission</th></tr>
        <tr>
            <th rowspan="3">CloseClient 1</th>
            <th rowspan="3">The client connection 1 that closes the specified connectid is connectid</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint8</th><th>Execution state 0 succeeded</th></tr>
        <tr>
            <th rowspan="3">ForbiddenIP -c 127.0.0.2 -t 1 -s 30 </th>
            <th rowspan="3">Disable the specified IP connection server, - c is IP, - t is type0 temporary 1 permanent - s is the blocking time, in seconds</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint8</th><th>Execution state 0 succeeded</th></tr>
        <tr>
            <th rowspan="8">ShowForbiddenIP -a</th>
            <th rowspan="8">View the banned IP list</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint32</th><th>banned IP counts</th></tr>
		<tr><th>uint8</th><th>banned IP length</th></tr>
		<tr><th>char</th><th>banned IP</th></tr>
		<tr><th>uint8</th><th>banned IP type 0 temporary 1 permanent</th></tr>
		<tr><th>uint32</th><th>Block start time, in the format 1970-01-01 to the current seconds</th></tr>
		<tr><th>uint32</th><th>Blocked seconds</th></tr>
        <tr>
            <th rowspan="3">LiftedIP 127.0.0.2</th>
            <th rowspan="3">Unblock IP</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
		<tr><th>uint8</th><th>Execution state 0 succeeded</th></tr>
        <tr>
            <th rowspan="16">ServerConnectTCP -a</th>
            <th rowspan="16">Inter - server communication ( TCP )</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint32</th><th>Total number of connections between servers</th></tr>
		<tr><th>uint8</th><th>Remote server IP length</th></tr>
		<tr><th>char</th><th>Remote server IP</th></tr>
		<tr><th>uint8</th><th>Connection current state 0 is connected 1 is not connected</th></tr>
		<tr><th>uint32</th><th>Connect ID</th></tr>
		<tr><th>uint32</th><th>Connect recv packet counts</th></tr>
		<tr><th>uint32</th><th>Connect send packet counts</th></tr>
		<tr><th>uint32</th><th>Connect recv packet sizes</th></tr>
		<tr><th>uint32</th><th>Connect send packet sizes</th></tr>
		<tr><th>uint32</th><th>Connection creation time, in the format 1970-01-01 to the current second value</th></tr>
		<tr><th>uint32</th><th>Connection active time in seconds</th></tr>
		<tr><th>uint32</th><th>Total number of connection receive queue packets</th></tr>
		<tr><th>uint64</th><th>Total connection reception elapsed time</th></tr>
		<tr><th>uint64</th><th>Total elapsed time for connection transmission</th></tr>
        <tr>
            <th rowspan="16">ServerConnectUDP -a</th>
            <th rowspan="16">Inter - server communication ( UDP )</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint32</th><th>Total number of connections between servers</th></tr>
		<tr><th>uint8</th><th>Remote server IP length</th></tr>
		<tr><th>char</th><th>Remote server IP</th></tr>
		<tr><th>uint8</th><th>Connection current state 0 is connected 1 is not connected</th></tr>
		<tr><th>uint32</th><th>Connect ID</th></tr>
		<tr><th>uint32</th><th>Connect recv packet counts</th></tr>
		<tr><th>uint32</th><th>Connect send packet counts</th></tr>
		<tr><th>uint32</th><th>Connect recv packet sizes</th></tr>
		<tr><th>uint32</th><th>Connect send packet sizes</th></tr>
		<tr><th>uint32</th><th>Connection creation time, in the format 1970-01-01 to the current second value</th></tr>
		<tr><th>uint32</th><th>Connection active time in seconds</th></tr>
		<tr><th>uint32</th><th>Total number of connection receive queue packets</th></tr>
		<tr><th>uint64</th><th>Total connection reception elapsed time</th></tr>
		<tr><th>uint64</th><th>Total elapsed time for connection transmission</th></tr>
        <tr>
            <th rowspan="7">ShowCurrProcessInfo -a</th>
            <th rowspan="7">View the current CPU, memory, and network health status of the server framework</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint32</th><th>Cpu operation ratio</th></tr>
		<tr><th>uint32</th><th>Current number of memory bytes occupied</th></tr>
		<tr><th>uint8</th><th>Flow switch status, 0 is off, 1 is on</th></tr>
		<tr><th>uint32</th><th>Server network throughput for the current minute(in)</th></tr>
		<tr><th>uint32</th><th>Server network throughput for the current minute(out)</th></tr>
        <tr>
            <th rowspan="8">ShowAllCommand -a</th>
            <th rowspan="8">View all message run information</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint32</th><th>Total number of message</th></tr>
		<tr><th>uint8</th><th>Module name length</th></tr>
		<tr><th>char</th><th>Module name</th></tr>
		<tr><th>uint16</th><th>Message ID</th></tr>
		<tr><th>uint32</th><th>Total current message processing</th></tr>
		<tr><th>uint32</th><th>Total current message processing time, in milliseconds</th></tr>
        <tr>
            <th rowspan="3">ReConnectServer -s 172.0.10.1</th>
            <th rowspan="3">Reconnect remote connection - s IP address of the remote server</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint8</th><th>Execution state 0 succeeded</th></tr>
        <tr>
            <th rowspan="3">SetDebug -s 0 </th>
            <th rowspan="3">Set debug file output status -s 0 on 1 off</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint8</th><th>Execution state 0 succeeded</th></tr>
        <tr>
            <th rowspan="3">ShowDebug -a</th>
            <th rowspan="3">Query debug file output status -s 0 on 1 off</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint8</th><th>0 open 1 close</th></tr>
        <tr>
            <th rowspan="6">GetConnectIPInfo -s 1</th>
            <th rowspan="6">The relevant IP information 1 obtain through that connectid is the connectid</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint16</th><th>0The corresponding IP information 1 was found and no corresponding IP information was found</th></tr>
		<tr><th>uint8</th><th>Current IP description length</th></tr>
		<tr><th>char</th><th>Current IP description</th></tr>
		<tr><th>uint32</th><th>IP port</th></tr>
        <tr>
            <th rowspan="10">GetLogInfo -a</th>
            <th rowspan="10">Get current log information</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint32</th><th>Number of logs</th></tr>
		<tr><th>uint32</th><th>Current log level</th></tr>
		<tr><th>uint16</th><th>Log ID</th></tr>
		<tr><th>uint8</th><th>Server name length</th></tr>
		<tr><th>char</th><th>Server name</th></tr>
		<tr><th>uint8</th><th>Log name length</th></tr>
		<tr><th>char</th><th>Log name</th></tr>
		<tr><th>uint8</th><th>Log output type 0 write file 1 write screen</th></tr>
        <tr>
            <th rowspan="3">SetLogLevel -l 1</th>
            <th rowspan="3">Set the log level -1 for level 1 or higher logs are not output</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint8</th><th>Execution state 0 succeeded</th></tr>
        <tr>
            <th rowspan="9">GetWorkThreadAI -a</th>
            <th rowspan="9">Get AI configuration information for thread</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint16</th><th>Current number of threads</th></tr>
		<tr><th>uint32</th><th>Work thread ID</th></tr>
		<tr><th>uint8</th><th>AI status 0 close 1 open</th></tr>
		<tr><th>uint32</th><th>Message packet dispose timeout seconds</th></tr>
		<tr><th>uint32</th><th>work thread timeout,in seconds</th></tr>
		<tr><th>uint32</th><th>work thread timeout counts limited</th></tr>
		<tr><th>uint32</th><th>stop message dispose time,in second</th></tr>
        <tr>
            <th rowspan="7">GetWorkThreadTO -a</th>
            <th rowspan="7">Gets all timeout packet information for thread</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint16</th><th>Current number of threads</th></tr>
		<tr><th>uint32</th><th>Work thread ID</th></tr>
		<tr><th>uint16</th><th>timeout message ID</th></tr>
		<tr><th>uint32</th><th>Timeout current time, in seconds since 1970-01-01</th></tr>
		<tr><th>uint32</th><th>Command execution time, in milliseconds</th></tr>
        <tr>
            <th rowspan="3">SetWorkThreadAI -i 1,1000,2000,10</th>
            <th rowspan="3">Set the configuration information for threadai, 1 to on, 1000 processing time limit 2000 check time limit 10 times to stop</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint8</th><th>Execution state 0 succeeded</th></tr>
        <tr>
            <th rowspan="9">GetNickNameInfo -n 127.0.0.1</th>
            <th rowspan="9">Obtain client IP information through IP alias</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
        <tr><th>uint32</th><th>IP counts</th></tr>
		<tr><th>uint32</th><th>Corresponding commandid</th></tr>
		<tr><th>uint8</th><th>IP length</th></tr>
		<tr><th>char</th><th>IP</th></tr>
		<tr><th>uint32</th><th>IP port</th></tr>
		<tr><th>uint8</th><th>Alias length</th></tr>
		<tr><th>char</th><th>Alias</th></tr>
		<tr><th>uint8</th><th>Does the front connection log open 0 not open 1 open</th></tr>
        <tr>
            <th rowspan="3">SetConnectLog -n 1 -f 0 </th>
            <th rowspan="3">Set connection log on state - n is connectid - f is 0 on 1 off</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint8</th><th>Execution state 0 succeeded</th></tr>
        <tr>
            <th rowspan="3">SetMaxConnectCount -n 20000 </th>
            <th rowspan="3">Sets the maximum number of connections allowed for the current server - n is the number of connections</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint8</th><th>Execution state 0 succeeded</th></tr>
        <tr>
            <th rowspan="3">AddListen -i 127.0.0.1 -p 10003 -t 1 -n 1</th>
            <th rowspan="3">Dynamically open a new listening port -i ip -p port -t 1 tcp 2 udp -n corresponding packet parseid on the server</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint8</th><th>Execution state 0 succeeded</th></tr>
        <tr>
            <th rowspan="3">DelListen -i 127.0.0.1 -p 10003 -t 1 </th>
            <th rowspan="3">Dynamically shut down the listening port -i ip -p port -t 1 tcp 2 udp on the server</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint8</th><th>Execution state 0 succeeded</th></tr>
        <tr>
            <th rowspan="6">ShowListen -a</th>
            <th rowspan="6">View existing open server port information</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint32</th><th>Number of listeners</th></tr>
		<tr><th>uint8</th><th>IP length</th></tr>
		<tr><th>char</th><th>IP</th></tr>
		<tr><th>uint32</th><th>IP port</th></tr>
        <tr>
            <th rowspan="3">TestFileStart -f a.xml</th>
            <th rowspan="3">Execute file pressure measurement instruction - f specifies the pressure measurement file name</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint8</th><th>Execution state 0 succeeded</th></tr>
        <tr>
            <th rowspan="3">TestFileStop -a</th>
            <th rowspan="3">Stop file pressure measurement</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint8</th><th>Execution state 0 succeeded</th></tr>
        <tr>
            <th rowspan="3">SetTrackIP -i 127.0.0.1 -c 5 </th>
            <th rowspan="3">Dyes the data for the specified client IP, -i is the client IP, -c is the number of dyed packets</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint8</th><th>Execution state 0 succeeded</th></tr>
        <tr>
            <th rowspan="3">SetTrackCommand -i 0x1000 -c 5 </th>
            <th rowspan="3">Dyes the data for the specified message id, -i message id in the format 0xxxxxx, -c is the number of dyed packets</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint8</th><th>Execution state 0 succeeded，1 failed</th></tr>
        <tr>
            <th rowspan="6">GetTrackCommandInfo -a</th>
            <th rowspan="6">View the message execution information for the current dye</th>
            <th>Type</th>
            <th>Explain</th>
        </tr>
		<tr><th>uint16</th><th>Return command ID</th></tr>
        <tr><th>uint8</th><th>Current number of message</th></tr>
		<tr><th>uint16</th><th>messageID</th></tr>
		<tr><th>uint16</th><th>Number of currently dyed message ( processed )</th></tr>
		<tr><th>uint16</th><th>Total number of dyes required for message</th></tr>
</table>

