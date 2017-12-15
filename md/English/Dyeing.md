# using data staining in PSS  
# # 1. trace features are designed for  
> considering that in actual use, we often encounter some users complain that the server response is slow.  
> for these problems, operation and maintenance is generally a direct hand r & d to deal with. Research and development can do most is to analyze user behavior according to the log.  
> this lookup mode is inefficient. In practice, if each feature appends a log, it slows down the program as a whole.  
> especially in high performance scenarios, this design is a big hidden danger.  
> however, without logging, when there is something wrong with the system, it is more difficult to troubleshoot.  
> so can the problem reflected in the operation and maintenance, by the operation and maintenance of this kind of self - examination?  
> this is the purpose of my design data dyeing, r & d in the process of actual plug-in development, in the key parts of their own code embedded some necessary " pile".  
> in the normal operation process, " pile" will not record anything ( the default is closed ), also won 't affect the operation of the program.  
> when some of these events occur, operation and maintenance can wake up these " pile" through the background tool, output runtime, execution code location, process, thread, and so on.  
> " pile" awakening, depends on the generation of a key, when a key generated and input into the" pile", the " pile" will execute the record code.  
> doing so provides a way to quickly find problems at minimal cost.  
> these " piles" can span processes, threads, and even machines.  
> the same key through the " pile" will be written to a file. These files will show the direction, location, and process of the data flow.  
> trace.h is a set of features provided to plug-ins and consists of three macros  
> ( 1 ) create_trace( x, y, z ) creates a key, x is the client IP, y is the port, z is the current signaling id ( key is a string of 48 characters )  
> ( 2 ) do_trace( x, y ) " pile", this code can be added anywhere in the plug-in code, x is the path to the output file, y is key, if key is empty," pile" do nothing.  
> ( 3 ) show_trace_id( x ) shows all the information in a key, including key generation time, IP, port, process id, thread id, signaling id  

# # 2. use of trace functionality  
> you can generate a key when you need it. This can be determined in your plug-in logic code.  
> when your key is generated, just enter the key where each function calls " pile" and the" pile" will execute the record.  
> for example, you in the plug-in domessage ( ) function, received a command, you can  
> string strKey = CREATE_TRACE(Clinet.IP, Client.port, Client.Command);    
> And then in your series of data processing process, add  
> DO_TRACE("./", strKey.c_str());  
> In some cases, we need to run time statistics on the data.  
> You can output the generation time of this key.  
> SHOW_TRACE_ID(strKey.c_str());  

# # 3. how does ops use PSS framework, dynamic data staining?
> according to the above principles, the framework supports two data dyeing mechanisms.
> ( 1 ) dye packets from a client IP source. ( specifies the total number of dyed packets, which are counted from the time the instruction is issued to stop )
> specifically, under PSS _ client manager or telnet, call settrackip to issue instructions to the PSS framework. [see here for specific operation documents](pssframecommand.md)
> ( 2 ) staining for a signaling. ( specifies the total number of dyed packets, which are counted from the time the instruction is issued to stop )
> specifically, under PSS _ client manager or telnet, call settrackip to issue instructions to the PSS framework. [see here for specific operation documents](pssframecommand.md) 