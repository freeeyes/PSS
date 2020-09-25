# PSS automation plug-in instructions
## 1. Purpose
> Reduce the development of plug-ins by developers. Automatically generate plug-in code based on XML.
> The developer configures the signaling, type, and structure required in the development of the plug-in through XML.
> XML2CPP will generate the corresponding plug-in original code according to your design.


## 2. How to do it
> (1)PlugInInfo.xml
> Open the PlugInInfo.xml file under the conf file
> Here is the main file of the plugin

```xml
<PlugIn desc="SmartShareBicycleBasic" keyID="SmartShareBicycleBasicID" Name="SmartShareBicycleBasic">
```
> There are several parameters here.
> desc is your plug-in description information (limited to 100 bytes)
> keyID KeyID of the plug-in (limited to 100 bytes)
> Name The name of the plug-in must be in English (limited to 100 bytes)

```xml
<Define DefName="UNLOCK_DOWN" type="number" DefValue="0x81" />
<Define DefName="LOCK_DOWN" type="number" DefValue="0x82" />
<Define DefName="PARAM_REPORT" type="number" DefValue="0x87" />
<Define DefName="VALIDATION_INFO_REPORT" type="number" DefValue="0x8B">
```

> Define here the signaling ID that you need to parse for the current plugin
> For example, your plug-in needs to process 0x81, 0x82, 0x87, 0x88 signaling messages.
> DefName The name of the macro definition corresponding to the signaling ID.
> Type can be number or string in the definition.
> Here you can define any macro you need in the plug-in.

```xml
<Command CommandIn="CLIENT_LINK_CONNECT" FuncName="Do_Connect">
</Command>

<Command CommandIn="CLIENT_LINK_CDISCONNET" FuncName="Do_DisConnect">
</Command>
```

> Define the events you need to handle here.
> CommandIn is one of the signaling IDs defined by the macro defined above (note that there must be a one-to-one correspondence).
> FuncName is the name of the processing function.
> For example, the above example defines the connection establishment and disconnection messages.
> Of course you can also define your own events.
> For example:


```xml
<Command CommandIn="PARAM_REPORT" FuncName="Do_Bicycle_Param_Report">
<Object class="Frame_Head" PacketType="HEAD" />
<Object class="Smart_Share_Bicycle_Param_Body" PacketType="BODY" />
<Object class="Smart_Share_Bicycle_Lock" PacketType="RETURN" />
</Command>
```

> An event is customized here. Process the PARAM_REPORT message.
> The receiving parameters here are divided into HEAD and BODY. Of course, these are not necessary, and you don't need to set these parameters.
> PacketType is divided into three types: HEAD (message header) BODY (message body) RETURN (data structure to be sent back)
> class is the corresponding class name, and the detailed definition needs to be defined in DataStruct.xml.

> (2) Definition of DataStruct.xml file
> Here define class objects and structures used by all programs.
> For example:
```xml
<Frame_Head desc="Intelligent bicycle report information header">
<StartCode desc="Start Code">uint16</StartCode>
<FunctionCode desc="function code">uint8</FunctionCode>
<BodyLength desc="package body length">uint8</BodyLength>
</Frame_Head>
```

> The header of the Frame_Head class is declared here.
> Here you can set the basic type of your class object.
> FunctionCode is the parameter name of the object inside
> The parameter type follows, and so on.

## 3, how to generate plug-ins
> After editing the above files, run XML2CPP (this tool can be run under windows and linux)
> After running successfully, a plug-in directory will be generated in the XML2CPP directory.
> For example, the plug-in name I defined above is SmartShareBicycleBasic.
> The plug-in I generated is the SmartShareBicycleBasic directory.
> This directory contains the C++ code of the plug-in.

## 4, how to add my code
> For plug-in programmers, just implement SmartShareBicycleBasic_Logic.cpp
> Fill in the relevant code.
> In this way, as the creator of the plug-in, you can restrict your plug-in implementation functions through configuration.
> In actual implementation, only the corresponding file SmartShareBicycleBasic_Logic.* needs to be implemented.
> Don't care how the data is sent and received.