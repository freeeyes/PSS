[![Build Status](https://travis-ci.org/freeeyes/PSS.svg?branch=master)](https://travis-ci.org/freeeyes/PSS)
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
[![GitHub version](https://badge.fury.io/gh/freeeyes%2FPSS.svg)](https://badge.fury.io/gh/freeeyes%2FPSS)
[![coverity](https://scan.coverity.com/projects/14425/badge.svg)](https://scan.coverity.com/projects/freeeyes-pss)
[![Platform](https://img.shields.io/badge/platform-Linux,%20Windows-green.svg?style=flat)](https://github.com/freeeyes/PSS)
 
>这是一个服务器框架。支持开发者使用插件(so或者dll)的方式，开发相关TCP和UDP的逻辑服务。 本服务框架基于ACE，可以在windows和linux下自适应编译运行。 本服务框架的宗旨是尽量剥离网络IO和逻辑开发者之间的关系，让逻辑开发者更专注于自己的业务，而网络IO部分完全由配置文件去实现。 运维管理者可以通过辅助的管理工具，获得框架运行状态，工作线程，数据流状态，连接状态等信息。辅助运维管理，问题排查。 另外，开发者还可以在完全脱离框架的基础上，利用框架周边工具，压力测试自己的逻辑模块，这样尽量减少上线前所可能出现的问题。 在使用框架前，你可以使用框架提供的小工具，压力测试当前框架的性能指标，作为是否采用的依据。 example下有专门的测试用例，可以提供开发者参考。 设计这个框架的目标不仅仅是一个网络IO的插件接口，而是一整套开发流程，尽量做到减少开发者的付出，规范开发过程（目前SVN上提供整套的框架测试工具，具体功能请参考先读我.txt） 希望能和大家一步步完善这个体系，真正做到有价值的框架。 我的目标是，你用的爽，就是成功。也希望大家越来越喜欢它。 另外最新代码会发布在SVN上，如果你有SVN，最好从SVN上直接下载，我会定时打版本包放在download里面提供下载。

## [PSS框架如何编译](Install.md) 
## [PSS框架配置说明](Configure.md)
## [PSS逻辑插件用例说明](examples.md)
## [后台指令集合](PSSFrameCommand.md) 
## [数据染色功能](Dyeing.md)
## [如何开发一个逻辑插件](LogicPlugin.md) 
## [如何开发一个解析插件](PacketParsePlugin.md) 
## [更新日志](Changelog.md)

>##提供几个压测数据（TcpPass工具，可从PSS开源工具包中获得）
> ###Windows平台
* * *
压测IP:172.21.1.200, 压测端口:10002.（Linux ContOS6 DEBUG）
* 压测类型:TCP
* 压测开始时间为: 2013-09-18 11:07:42
* 压测结束时间为: 2013-09-18 12:06:00
* 压测线程数:10
* 创建成功连接数:10
* 成功发送数据包数:11675202
* 成功接收数据包数:11675202
* 连接失败数:0
* 发送失败数据包数:0
* 接收失败数据包数:0
* 连接成功百分比:100.000000%
* 发送数据包成功百分比:100.000000%
* 接收数据包成功百分比:100.000000%

* * * 
> ###Linux平台
* 压测IP:172.21.1.68, 压测端口:10002.（Windows7 DEBUG）
* 压测类型:TCP
* 压测开始时间为: 2013-09-16 09:55:58
* 压测结束时间为: 2013-09-16 10:38:09
* 压测线程数:10
* 创建成功连接数:10
* 成功发送数据包数:11514132
* 成功接收数据包数:11514132
* 连接失败数:3125
* 发送失败数据包数:0
* 接收失败数据包数:0
* 连接成功百分比:100.000000%
* 发送数据包成功百分比:100.000000%
* 接收数据包成功百分比:100.000000%

* * *

作者:
freeeyes

框架贡献者:
凹凸man,w1w,乔戈,Bobo,弄香花满衣,崔,kz

QQ框架技术讨论群: 260862613

