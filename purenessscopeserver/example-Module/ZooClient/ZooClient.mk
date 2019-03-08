##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=ZooClient
ConfigurationName      :=Release
WorkspacePath          :=/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite
ProjectPath            :=/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite/ZooClient
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
OutputFile             :=bin/Release/ZooClient
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="ZooClient.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../../../purenessscopeserver/Common $(IncludeSwitch)../../../../../zookeeper-release-3.5.4/src/c/include 
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
CXXFLAGS :=  -O2 -fPIC -DTHREADED -Wall -fexceptions  $(Preprocessors)
CFLAGS   :=  -O2 -fPIC -DTHREADED -Wall -fexceptions  $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_ZooClient.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_zkClient.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_BaseCommand.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite/.build-release/ZooClient"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_ZooClient.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/ZooClient/ZooClient.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_ZooClient.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/ZooClient/ZooClient.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_ZooClient.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_ZooClient.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/ZooClient/ZooClient.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_ZooClient.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_ZooClient.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/ZooClient/ZooClient.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_ZooClient.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/ZooClient/ZooClient.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_ZooClient.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/ZooClient/ZooClient.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_zkClient.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/ZooClient/zkClient.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_zkClient.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/ZooClient/zkClient.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_zkClient.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_zkClient.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/ZooClient/zkClient.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_zkClient.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_zkClient.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/ZooClient/zkClient.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_zkClient.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/ZooClient/zkClient.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_zkClient.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/ZooClient/zkClient.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_BaseCommand.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/ZooClient/BaseCommand.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_BaseCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/ZooClient/BaseCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_BaseCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_BaseCommand.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/ZooClient/BaseCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_BaseCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_BaseCommand.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/ZooClient/BaseCommand.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_BaseCommand.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/ZooClient/BaseCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_ZooClient_BaseCommand.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/ZooClient/BaseCommand.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


