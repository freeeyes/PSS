##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=Proxy_Proxy
ConfigurationName      :=Release
WorkspacePath          :=/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite
ProjectPath            :=/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite/Proxy_Proxy
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=fox mulder
Date                   :=2019年03月06日
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
OutputFile             :=bin/Release/Proxy_Proxy
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="Proxy_Proxy.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../../../purenessscopeserver/Common 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)Debug 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -O2 -fPIC -Wall -fexceptions  $(Preprocessors)
CFLAGS   :=  -O2 -fPIC -Wall -fexceptions  $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_ProxyClient.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_BaseCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_ProxyTest.cpp$(ObjectSuffix) 



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
	$(SharedObjectLinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)
	@$(MakeDirCommand) "/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite/.build-release"
	@echo rebuilt > "/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite/.build-release/Proxy_Proxy"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_ProxyClient.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/Proxy/Proxy/ProxyClient.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_ProxyClient.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/Proxy/Proxy/ProxyClient.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_ProxyClient.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_ProxyClient.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/Proxy/Proxy/ProxyClient.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_ProxyClient.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_ProxyClient.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/Proxy/Proxy/ProxyClient.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_ProxyClient.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/Proxy/Proxy/ProxyClient.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_ProxyClient.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/Proxy/Proxy/ProxyClient.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_BaseCommand.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/Proxy/Proxy/BaseCommand.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_BaseCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/Proxy/Proxy/BaseCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_BaseCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_BaseCommand.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/Proxy/Proxy/BaseCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_BaseCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_BaseCommand.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/Proxy/Proxy/BaseCommand.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_BaseCommand.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/Proxy/Proxy/BaseCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_BaseCommand.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/Proxy/Proxy/BaseCommand.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_ProxyTest.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/Proxy/Proxy/ProxyTest.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_ProxyTest.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/Proxy/Proxy/ProxyTest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_ProxyTest.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_ProxyTest.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/Proxy/Proxy/ProxyTest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_ProxyTest.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_ProxyTest.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/Proxy/Proxy/ProxyTest.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_ProxyTest.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/Proxy/Proxy/ProxyTest.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_Proxy_Proxy_ProxyTest.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/Proxy/Proxy/ProxyTest.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


