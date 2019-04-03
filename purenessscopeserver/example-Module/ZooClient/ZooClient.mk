##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=ZooClient
ConfigurationName      :=Release
WorkspacePath          :=/data/develop/Lib/Network/PSS-master/Build/Linux
ProjectPath            :=/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/ZooClient
IntermediateDirectory  :=$(WorkspacePath)/Linux_Bin/Temp
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
OutputFile             :=$(WorkspacePath)/Linux_Bin/Module/lib$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="ZooClient.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../../purenessscopeserver/Common $(IncludeSwitch)../../../ThirdLib/thirdparty/zookeeper/include/ 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)Release 

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
Objects0=$(IntermediateDirectory)/ZooClient_ZooClient.cpp$(ObjectSuffix) $(IntermediateDirectory)/ZooClient_BaseCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/ZooClient_zkClient.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "/data/develop/Lib/Network/PSS-master/Build/Linux/.build-release"
	@echo rebuilt > "/data/develop/Lib/Network/PSS-master/Build/Linux/.build-release/ZooClient"

MakeIntermediateDirs:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp


$(IntermediateDirectory)/.d:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/ZooClient_ZooClient.cpp$(ObjectSuffix): ./ZooClient/ZooClient.cpp $(IntermediateDirectory)/ZooClient_ZooClient.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/ZooClient/ZooClient/ZooClient.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ZooClient_ZooClient.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ZooClient_ZooClient.cpp$(DependSuffix): ./ZooClient/ZooClient.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ZooClient_ZooClient.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ZooClient_ZooClient.cpp$(DependSuffix) -MM ./ZooClient/ZooClient.cpp

$(IntermediateDirectory)/ZooClient_ZooClient.cpp$(PreprocessSuffix): ./ZooClient/ZooClient.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ZooClient_ZooClient.cpp$(PreprocessSuffix) ./ZooClient/ZooClient.cpp

$(IntermediateDirectory)/ZooClient_BaseCommand.cpp$(ObjectSuffix): ./ZooClient/BaseCommand.cpp $(IntermediateDirectory)/ZooClient_BaseCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/ZooClient/ZooClient/BaseCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ZooClient_BaseCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ZooClient_BaseCommand.cpp$(DependSuffix): ./ZooClient/BaseCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ZooClient_BaseCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ZooClient_BaseCommand.cpp$(DependSuffix) -MM ./ZooClient/BaseCommand.cpp

$(IntermediateDirectory)/ZooClient_BaseCommand.cpp$(PreprocessSuffix): ./ZooClient/BaseCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ZooClient_BaseCommand.cpp$(PreprocessSuffix) ./ZooClient/BaseCommand.cpp

$(IntermediateDirectory)/ZooClient_zkClient.cpp$(ObjectSuffix): ./ZooClient/zkClient.cpp $(IntermediateDirectory)/ZooClient_zkClient.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/ZooClient/ZooClient/zkClient.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ZooClient_zkClient.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ZooClient_zkClient.cpp$(DependSuffix): ./ZooClient/zkClient.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ZooClient_zkClient.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ZooClient_zkClient.cpp$(DependSuffix) -MM ./ZooClient/zkClient.cpp

$(IntermediateDirectory)/ZooClient_zkClient.cpp$(PreprocessSuffix): ./ZooClient/zkClient.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ZooClient_zkClient.cpp$(PreprocessSuffix) ./ZooClient/zkClient.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(WorkspacePath)/Linux_Bin/Temp/


