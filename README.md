English | [简体中文](./README_Chinese.md)

<p align="right">Current stable version: <strong>v2.1.0</strong></p>
<p align="center"><img src="https://raw.githubusercontent.com/freeeyes/PSS/gh-pages/_images/pss.svg?sanitize=true" alt="pss" width="380"/></p>
<center>This is a based plug-in framework that provides cross-platform IO and logically separated plug-in services.  

----------

[![Build Status](https://travis-ci.com/freeeyes/PSS.svg?branch=master)](https://travis-ci.com/freeeyes/PSS)
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
[![GitHub version](https://badge.fury.io/gh/freeeyes%2FPSS.svg)](https://badge.fury.io/gh/freeeyes%2FPSS)
[![Quality Gate](https://sonarcloud.io/api/project_badges/measure?project=freeeyes&metric=alert_status)](https://sonarcloud.io/dashboard?id=freeeyes)
[![Platform](https://img.shields.io/badge/platform-Linux,%20Windows-green.svg?style=flat)](https://github.com/freeeyes/PSS)
</center>

## [A better version based on asio PSS3.0](https://github.com/ArkNX/PSS_ASIO)  

# OverView
this is a server framework. Support the way developers use plug-ins ( so or dll library).  
develop logical services for related TCP and UDP. This service framework is based on ace and can be compiled and run adaptively under windows and Linux.

the purpose of this service framework is to peel off the relationship between network io and logical developers as much as possible, so that the logical developers are more focused on their own business, and the network io is fully implemented by the configuration file.

operation and maintenance manager can through the auxiliary management tools, to obtain the framework operation status, work thread, data flow status, connection status and other information.  
auxiliary operation and maintenance management, troubleshooting.

in addition, developers can also be completely out of the framework, using the tools around the framework, pressure test their logic module, so as to minimize the possible problems before online.

before using the framework, you can use the framework to provide small tools, pressure test performance indicators of the current framework, as a basis for whether to adopt.

example has a special test case, can provide a developer reference.

the goal of this framework is not just a network io plug-in interface, but a complete set of development process, as far as possible to reduce the developer 's pay.

specification development process ( currently available on the SVN framework testing tools, specific functions please refer to other instructions MD )

hope to be able to step by step with you to improve the system, really do a valuable framework. My goal is, you use cool, is to succeed. And I hope you like it more and more.

## [How does the PSS framework compile](./Doc/English/Install.md) 
## [Pss framework configuration instructions](./Doc/English/Configure.md)
## [Pss logical plug - in use case description](./Doc/English/examples.md)
## [Pss Background commands instruction](./Doc/English/PSSFrameCommand.md) 
## [How to Data dyeing](./Doc/English/Dyeing.md)
## [How to develop a logical plugin](./Doc/English/LogicPlugin.md) 
## [How to develop a parsing plugin](./Doc/English/PacketParsePlugin.md) 
## [How to automatically generate a plugin](./Doc/English/PluginMake.md)

![pss frame flow chart](http://on-img.com/chart_image/5a6ae014e4b0d1c5b5b1e6fa.png)

> **Provides several piezometric data ( TCP pass tool available from PSS open source kit )**


Author & Thanks: 
凹凸man,w1w,乔戈,Bobo,弄香花满衣,崔,kz,瑞奇

QQ group: 260862613