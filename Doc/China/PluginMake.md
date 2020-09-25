# PSS自动化插件使用说明
## 1，目的
> 减少开发人员对插件的开发。根据XML自动化生成插件代码。   
> 开发人员通过XML配置插件的开发中需要的信令，类型，结构。  
> XML2CPP会根据你的设计生成对应的插件原始代码。  


## 2，如何做
> (1)PlugInInfo.xml  
> 打开在conf文件下的这个PlugInInfo.xml文件  
> 这里是插件的主要文件

```xml
<PlugIn desc="SmartShareBicycleBasic" keyID="SmartShareBicycleBasicID" Name="SmartShareBicycleBasic">
```
> 这里有几个参数。  
> desc 是你的插件描述信息(限制在100字节)  
> keyID 插件的KeyID(限制在100字节)  
> Name 插件的名字 必须是英文(限制在100字节)  

```xml
<Define DefName="UNLOCK_DOWN" type="number" DefValue="0x81" />
<Define DefName="LOCK_DOWN" type="number" DefValue="0x82" />
<Define DefName="PARAM_REPORT" type="number" DefValue="0x87" />
<Define DefName="VALIDATION_INFO_REPORT" type="number" DefValue="0x8B" >
```

> 在这里定义当前插件你需要解析的信令ID  
> 比如，你的插件需要处理0x81,0x82,0x87,0x88信令的消息。  
> DefName 对应信令ID的宏定义的名称。  
> type 在定义这里可以是number或者string。  
> 这里可以定义插件中你需要的任意的宏。  

```xml
<Command CommandIn="CLIENT_LINK_CONNECT" FuncName="Do_Connect">
</Command>		

<Command CommandIn="CLIENT_LINK_CDISCONNET" FuncName="Do_DisConnect">
</Command>	
```

> 在这里定义你的需要处理的事件。  
> CommandIn 是上面你定义的宏定义的信令ID之一(注意必须一一对应)。  
> FuncName 是处理函数的名称。  
> 比如上述的例子是定义了连接建立和断开连接的消息。  
> 当然你也可以定义自己的事件。  
> 例如如下：  


```xml
<Command CommandIn="PARAM_REPORT" FuncName="Do_Bicycle_Param_Report">
<Object class="Frame_Head" PacketType="HEAD" />
<Object class="Smart_Share_Bicycle_Param_Body" PacketType="BODY" />
<Object class="Smart_Share_Bicycle_Lock" PacketType="RETURN" />
</Command>
```

> 这里自定义了一个事件。处理PARAM_REPORT的消息。  
> 这里接收参数分为HEAD和BODY，当然，这些不是必须的，你也可以不设置这些参数。
> PacketType类型分为三种 HEAD(消息头) BODY(消息体) RETURN(要发送返回的数据结构)  
> class是对应的类名称,详细的定义需要在DataStruct.xml里面定义。  

> (2)DataStruct.xml文件的定义  
> 这里定义所有程序用到的类对象和结构体。  
> 例如：  
```xml
<Frame_Head desc="智享单车上报信息头">
<StartCode desc="起始码">uint16</StartCode>
<FunctionCode desc="功能码">uint8</FunctionCode>
<BodyLength desc="包体长度">uint8</BodyLength>
</Frame_Head>
```

> 这里声明了Frame_Head类的头。  
> 这里可以设置你的类对象的基础类型。  
> FunctionCode 为里面对象的参数名称  
> 后面是参数类型, 以此类推。

## 3，如何生成插件
> 上述的文件，编辑完毕后，运行XML2CPP(此工具可以在windows和linux下运行)  
> 运行成功后，会在XML2CPP目录下生成一个插件目录。  
> 比如上述我定义的插件名称是SmartShareBicycleBasic。  
> 我生成的插件就是SmartShareBicycleBasic目录。  
> 这个目录里面包含了插件的C++代码。  

## 4，如何添加我的代码
> 对于插件程序员，只要实现 SmartShareBicycleBasic_Logic.cpp  
> 相关的代码填充即可。  
> 这样，作为插件的创造者，可以通过配置来约束自己的插件实现功能。  
> 而实际实现着，只需要实现对应的文件SmartShareBicycleBasic_Logic.*。  
> 不用管数据是如何收发的。