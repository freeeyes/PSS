##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=RealTimeMonitor
ConfigurationName      :=Release
WorkspacePath          :=../../../Build/Linux
ProjectPath            :=../../purenessscopeserver/example-Module/RealTimeMonitor
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
ObjectsFileList        :="RealTimeMonitor.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../../purenessscopeserver/Common 
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
CXXFLAGS :=  -O2 -fPIC -Wall -fexceptions  $(Preprocessors)
CFLAGS   :=  -O2 -fPIC -Wall -fexceptions  $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/RealTimeMonitor_RealTimeMonitor.cpp$(ObjectSuffix) $(IntermediateDirectory)/RealTimeMonitor_cJSON.cpp$(ObjectSuffix) $(IntermediateDirectory)/RealTimeMonitor_PostMonitorData.cpp$(ObjectSuffix) $(IntermediateDirectory)/RealTimeMonitor_MonitorCommand.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "../../../Build/Linux/.build-release"
	@echo rebuilt > "../../../Build/Linux/.build-release/RealTimeMonitor"

MakeIntermediateDirs:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp


$(IntermediateDirectory)/.d:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/RealTimeMonitor_RealTimeMonitor.cpp$(ObjectSuffix): ./RealTimeMonitor/RealTimeMonitor.cpp $(IntermediateDirectory)/RealTimeMonitor_RealTimeMonitor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./RealTimeMonitor/RealTimeMonitor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RealTimeMonitor_RealTimeMonitor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RealTimeMonitor_RealTimeMonitor.cpp$(DependSuffix): ./RealTimeMonitor/RealTimeMonitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RealTimeMonitor_RealTimeMonitor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RealTimeMonitor_RealTimeMonitor.cpp$(DependSuffix) -MM ./RealTimeMonitor/RealTimeMonitor.cpp

$(IntermediateDirectory)/RealTimeMonitor_RealTimeMonitor.cpp$(PreprocessSuffix): ./RealTimeMonitor/RealTimeMonitor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RealTimeMonitor_RealTimeMonitor.cpp$(PreprocessSuffix) ./RealTimeMonitor/RealTimeMonitor.cpp

$(IntermediateDirectory)/RealTimeMonitor_cJSON.cpp$(ObjectSuffix): ./RealTimeMonitor/cJSON.cpp $(IntermediateDirectory)/RealTimeMonitor_cJSON.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./RealTimeMonitor/cJSON.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RealTimeMonitor_cJSON.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RealTimeMonitor_cJSON.cpp$(DependSuffix): ./RealTimeMonitor/cJSON.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RealTimeMonitor_cJSON.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RealTimeMonitor_cJSON.cpp$(DependSuffix) -MM ./RealTimeMonitor/cJSON.cpp

$(IntermediateDirectory)/RealTimeMonitor_cJSON.cpp$(PreprocessSuffix): ./RealTimeMonitor/cJSON.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RealTimeMonitor_cJSON.cpp$(PreprocessSuffix) ./RealTimeMonitor/cJSON.cpp

$(IntermediateDirectory)/RealTimeMonitor_PostMonitorData.cpp$(ObjectSuffix): ./RealTimeMonitor/PostMonitorData.cpp $(IntermediateDirectory)/RealTimeMonitor_PostMonitorData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./RealTimeMonitor/PostMonitorData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RealTimeMonitor_PostMonitorData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RealTimeMonitor_PostMonitorData.cpp$(DependSuffix): ./RealTimeMonitor/PostMonitorData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RealTimeMonitor_PostMonitorData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RealTimeMonitor_PostMonitorData.cpp$(DependSuffix) -MM ./RealTimeMonitor/PostMonitorData.cpp

$(IntermediateDirectory)/RealTimeMonitor_PostMonitorData.cpp$(PreprocessSuffix): ./RealTimeMonitor/PostMonitorData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RealTimeMonitor_PostMonitorData.cpp$(PreprocessSuffix) ./RealTimeMonitor/PostMonitorData.cpp

$(IntermediateDirectory)/RealTimeMonitor_MonitorCommand.cpp$(ObjectSuffix): ./RealTimeMonitor/MonitorCommand.cpp $(IntermediateDirectory)/RealTimeMonitor_MonitorCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./RealTimeMonitor/MonitorCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RealTimeMonitor_MonitorCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RealTimeMonitor_MonitorCommand.cpp$(DependSuffix): ./RealTimeMonitor/MonitorCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RealTimeMonitor_MonitorCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RealTimeMonitor_MonitorCommand.cpp$(DependSuffix) -MM ./RealTimeMonitor/MonitorCommand.cpp

$(IntermediateDirectory)/RealTimeMonitor_MonitorCommand.cpp$(PreprocessSuffix): ./RealTimeMonitor/MonitorCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RealTimeMonitor_MonitorCommand.cpp$(PreprocessSuffix) ./RealTimeMonitor/MonitorCommand.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(WorkspacePath)/Linux_Bin/Temp/


