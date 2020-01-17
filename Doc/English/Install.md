# PSS compilation process  
## (1) installation of third party software ace  
> this program needs to rely on ace to run, please go to[http://download.dre.vanderbilt.edu/ "ACE url") Download the ace runtime。  
> Compile the tutorial, see[http://www.cs.wustl.edu/~schmidt/ACE-install.html](http://www.cs.wustl.edu/~schmidt/ACE-install.html "ACE install introduction")  
> Or follow these steps to add the environment variable first.    
> vi .bash_profile  
> `ACE_ROOT=/Software/ACE_wrappers      ------Is the directory on which ace decompression source files are stored`  
> `export ACE_ROOT`  
> `LD_LIBARY_PATH=$ACE_ROOT/ace:$LD_LIBRARY_PATH   -----Add the library of ace to the environment variable LD_lIBRARY_PATH`   
> `export LD_LIBRARY_PATH`  
> `export PATH=$PATH:$ACE_ROOT/bin  --Specifies the run path of the MPC`    
> You can create a config.h in the ace directory first    
> add code
> `#include "ace/config-linux.h"`    
> and then in $ACE_ROOT/include/makeinclude  
> create platform_macros.GNU，and add code：  
> `include $(ACE_ROOT)/include/makeinclude/platform_linux.GNU`  
> and back ace path  
> make  
> make install  
> After the compilation is complete, check that libace.so exists under the previous lib folder, and compile succeeds if it exists. 
> In addition, if you use the epoll model under Linux 
> Please configure /etc/security/limits.conf。  
> add code  
> `*        soft    noproc  10000`  
> `*        hard    noproc  10000`  
> `*        soft    nofile  10000`  
> `*        hard    nofile  10000`   
## (2) PSS compilation and installation ( Linux, for example, Windows please use CMake command to generate the project directly, the program has a special engineering file, open the compilation )  
> download PSS and unzip it under the pureness scope server / Linux _ bin path.  
> first set the script run permissions  
> `chmod 775 runlinuxmpc.sh` 
> `./runlinuxmpc.sh` 
> Normally, this script automatically generates makefile files.    
> and run
> `make` 
> Similarly, go to the pureness scope server / packet parse_interface directory. ( message resolution plug - in )  
> `chmod 775 runlinuxmake.sh`  
> `./runlinuxmake.sh`
> `make` 
> If you want to test the framework, you can compile the tcptest module. ( message processing plug - in )   
> in path purenessscopeserver/example-Module/TcpTest  
> `chmod 775 runlinuxmake.sh`  
> `./runlinuxmake.sh`
> `make` 
> After all plug-ins have been compiled.  
> The following files are visible under pureness scope server /Linux_bin。  
> libPacketParse_Interface.so  
> libTcpTest.so  
> PurenessScopeServer  
> Run. /purenessscopeserver to start successfully.
## (3) You can directly execute the script one-click installation under Linux  
> Run configure under ./Build directory  
> Source ~ / .bash_profile after running  

