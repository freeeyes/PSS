##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=RealTimeMonitor
ConfigurationName      :=Release
WorkspacePath          :=/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite
ProjectPath            :=/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite/RealTimeMonitor
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
OutputFile             :=bin/Release/libRealTimeMonitor
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="RealTimeMonitor.txt"
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
Objects0=$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_RealTimeMonitor.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_cJSON.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_PostMonitorData.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_MonitorCommand.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite/.build-release/RealTimeMonitor"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_RealTimeMonitor.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/RealTimeMonitor/RealTimeMonitor.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_RealTimeMonitor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/RealTimeMonitor/RealTimeMonitor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_RealTimeMonitor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_RealTimeMonitor.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/RealTimeMonitor/RealTimeMonitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_RealTimeMonitor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_RealTimeMonitor.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/RealTimeMonitor/RealTimeMonitor.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_RealTimeMonitor.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/RealTimeMonitor/RealTimeMonitor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_RealTimeMonitor.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/RealTimeMonitor/RealTimeMonitor.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_cJSON.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/RealTimeMonitor/cJSON.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_cJSON.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/RealTimeMonitor/cJSON.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_cJSON.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_cJSON.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/RealTimeMonitor/cJSON.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_cJSON.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_cJSON.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/RealTimeMonitor/cJSON.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_cJSON.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/RealTimeMonitor/cJSON.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_cJSON.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/RealTimeMonitor/cJSON.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_PostMonitorData.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/RealTimeMonitor/PostMonitorData.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_PostMonitorData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/RealTimeMonitor/PostMonitorData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_PostMonitorData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_PostMonitorData.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/RealTimeMonitor/PostMonitorData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_PostMonitorData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_PostMonitorData.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/RealTimeMonitor/PostMonitorData.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_PostMonitorData.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/RealTimeMonitor/PostMonitorData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_PostMonitorData.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/RealTimeMonitor/PostMonitorData.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_MonitorCommand.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/RealTimeMonitor/MonitorCommand.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_MonitorCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/RealTimeMonitor/MonitorCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_MonitorCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_MonitorCommand.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/RealTimeMonitor/MonitorCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_MonitorCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_MonitorCommand.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/RealTimeMonitor/MonitorCommand.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_MonitorCommand.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/RealTimeMonitor/MonitorCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_RealTimeMonitor_MonitorCommand.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/RealTimeMonitor/MonitorCommand.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


