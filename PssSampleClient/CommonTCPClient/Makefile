##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=CommonTCPClient
ConfigurationName      :=Release
WorkspacePath          :=../../Build/Linux
ProjectPath            :=../PssSampleClient/CommonTCPClient
IntermediateDirectory  :=$(WorkspacePath)/Temp
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=fox mulder
Date                   :=2019年03月30日
CodeLitePath           :=/home/fox-mulder/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(WorkspacePath)/Linux_Bin/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)NDEBUG $(PreprocessorSwitch)_LINUX_PLATFORM_ 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="CommonTCPClient.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -O2
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../../ThirdLib/include $(IncludeSwitch)../../ThirdLib/include/TinyXML $(IncludeSwitch)../../ThirdLib/thirdparty 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)pthread 
ArLibs                 :=  "libpthread.so" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)/usr/lib/ 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=   $(Preprocessors)
CFLAGS   :=   $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/up_up_ThirdLib_thirdparty_sock_wrap.cpp$(ObjectSuffix) $(IntermediateDirectory)/CommonTCPClient_TpClientManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/CommonTCPClient_CommonTCPClient.cpp$(ObjectSuffix) $(IntermediateDirectory)/CommonTcpTest_CommonTcpTest.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d $(WorkspacePath)/Temp || $(MakeDirCommand) $(WorkspacePath)/Temp


$(IntermediateDirectory)/.d:
	@test -d $(WorkspacePath)/Temp || $(MakeDirCommand) $(WorkspacePath)/Temp

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_up_ThirdLib_thirdparty_sock_wrap.cpp$(ObjectSuffix): ../../ThirdLib/thirdparty/sock_wrap.cpp $(IntermediateDirectory)/up_up_ThirdLib_thirdparty_sock_wrap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../../ThirdLib/thirdparty/sock_wrap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_ThirdLib_thirdparty_sock_wrap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_ThirdLib_thirdparty_sock_wrap.cpp$(DependSuffix): ../../ThirdLib/thirdparty/sock_wrap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_ThirdLib_thirdparty_sock_wrap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_ThirdLib_thirdparty_sock_wrap.cpp$(DependSuffix) -MM ../../ThirdLib/thirdparty/sock_wrap.cpp

$(IntermediateDirectory)/up_up_ThirdLib_thirdparty_sock_wrap.cpp$(PreprocessSuffix): ../../ThirdLib/thirdparty/sock_wrap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_ThirdLib_thirdparty_sock_wrap.cpp$(PreprocessSuffix) ../../ThirdLib/thirdparty/sock_wrap.cpp

$(IntermediateDirectory)/CommonTCPClient_TpClientManager.cpp$(ObjectSuffix): CommonTCPClient/TpClientManager.cpp $(IntermediateDirectory)/CommonTCPClient_TpClientManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./CommonTCPClient/TpClientManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CommonTCPClient_TpClientManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CommonTCPClient_TpClientManager.cpp$(DependSuffix): CommonTCPClient/TpClientManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CommonTCPClient_TpClientManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CommonTCPClient_TpClientManager.cpp$(DependSuffix) -MM CommonTCPClient/TpClientManager.cpp

$(IntermediateDirectory)/CommonTCPClient_TpClientManager.cpp$(PreprocessSuffix): CommonTCPClient/TpClientManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CommonTCPClient_TpClientManager.cpp$(PreprocessSuffix) CommonTCPClient/TpClientManager.cpp

$(IntermediateDirectory)/CommonTCPClient_CommonTCPClient.cpp$(ObjectSuffix): CommonTCPClient/CommonTCPClient.cpp $(IntermediateDirectory)/CommonTCPClient_CommonTCPClient.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./CommonTCPClient/CommonTCPClient.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CommonTCPClient_CommonTCPClient.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CommonTCPClient_CommonTCPClient.cpp$(DependSuffix): CommonTCPClient/CommonTCPClient.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CommonTCPClient_CommonTCPClient.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CommonTCPClient_CommonTCPClient.cpp$(DependSuffix) -MM CommonTCPClient/CommonTCPClient.cpp

$(IntermediateDirectory)/CommonTCPClient_CommonTCPClient.cpp$(PreprocessSuffix): CommonTCPClient/CommonTCPClient.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CommonTCPClient_CommonTCPClient.cpp$(PreprocessSuffix) CommonTCPClient/CommonTCPClient.cpp

$(IntermediateDirectory)/CommonTcpTest_CommonTcpTest.cpp$(ObjectSuffix): CommonTcpTest/CommonTcpTest.cpp $(IntermediateDirectory)/CommonTcpTest_CommonTcpTest.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./CommonTcpTest/CommonTcpTest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CommonTcpTest_CommonTcpTest.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CommonTcpTest_CommonTcpTest.cpp$(DependSuffix): CommonTcpTest/CommonTcpTest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CommonTcpTest_CommonTcpTest.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CommonTcpTest_CommonTcpTest.cpp$(DependSuffix) -MM CommonTcpTest/CommonTcpTest.cpp

$(IntermediateDirectory)/CommonTcpTest_CommonTcpTest.cpp$(PreprocessSuffix): CommonTcpTest/CommonTcpTest.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CommonTcpTest_CommonTcpTest.cpp$(PreprocessSuffix) CommonTcpTest/CommonTcpTest.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(WorkspacePath)/Temp/


