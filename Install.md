# PSS编译过程  
## (1)第三方软件ACE的安装  
> 本程序需要依赖ACE才能运行，请到[http://www.cs.wustl.edu/~schmidt/ACE.html](http://www.cs.wustl.edu/~schmidt/ACE.html "ACE官网")下载ACE的运行库。  
> 编译教程请参考[http://www.cs.wustl.edu/~schmidt/ACE-install.html](http://www.cs.wustl.edu/~schmidt/ACE-install.html "ACE安装说明文档地址")  
> 异或按照如下步骤进行，首先添加环境变量。  
> vi .bash_profile  
> `ACE_ROOT=/Software/ACE_wrappers      ------就是上面存放ACE解压源文件的目录  
export ACE_ROOT  

LD_LIBARY_PATH=$ACE_ROOT/ace:$LD_LIBRARY_PATH   -----把ACE的库添加到环境变量LD_LIBARY_PATH中去   
export LD_LIBRARY_PATH  

export PATH=$PATH:$ACE_ROOT/bin  --指定MPC的运行路径`    
> 你可以先在ace目录下创建一个config.h  
> 添加如下代码
> `#include "ace/config-linux.h"`    
> 然后进入$ACE_ROOT/include/makeinclude  
> 新建 platform_macros.GNU，在中加入一行如 下：  
> `include $(ACE_ROOT)/include/makeinclude/platform_linux.GNU`  
> 然后回到ace目录  
> make  
> make install  
> 编译完成后，检查上一层lib文件夹下的libace.so是否存在，存在则编译成功。  
## (2)PSS的编译安装  



