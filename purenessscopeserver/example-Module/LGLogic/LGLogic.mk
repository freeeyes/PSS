##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=LGLogic
ConfigurationName      :=Release
WorkspacePath          :=/data/develop/Lib/Network/PSS-master/Build/Linux
ProjectPath            :=/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/LGLogic
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
ObjectsFileList        :="LGLogic.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../../purenessscopeserver/Common 
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
Objects0=$(IntermediateDirectory)/LGLogic_BaseCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/LGLogic_LGLogic.cpp$(ObjectSuffix) $(IntermediateDirectory)/LGLogic_LSServerManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/LGLogic_ListManager.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/data/develop/Lib/Network/PSS-master/Build/Linux/.build-release/LGLogic"

MakeIntermediateDirs:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp


$(IntermediateDirectory)/.d:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/LGLogic_BaseCommand.cpp$(ObjectSuffix): ./LGLogic/BaseCommand.cpp $(IntermediateDirectory)/LGLogic_BaseCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/LGLogic/LGLogic/BaseCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LGLogic_BaseCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LGLogic_BaseCommand.cpp$(DependSuffix): ./LGLogic/BaseCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LGLogic_BaseCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LGLogic_BaseCommand.cpp$(DependSuffix) -MM ./LGLogic/BaseCommand.cpp

$(IntermediateDirectory)/LGLogic_BaseCommand.cpp$(PreprocessSuffix): ./LGLogic/BaseCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LGLogic_BaseCommand.cpp$(PreprocessSuffix) ./LGLogic/BaseCommand.cpp

$(IntermediateDirectory)/LGLogic_LGLogic.cpp$(ObjectSuffix): ./LGLogic/LGLogic.cpp $(IntermediateDirectory)/LGLogic_LGLogic.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/LGLogic/LGLogic/LGLogic.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LGLogic_LGLogic.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LGLogic_LGLogic.cpp$(DependSuffix): ./LGLogic/LGLogic.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LGLogic_LGLogic.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LGLogic_LGLogic.cpp$(DependSuffix) -MM ./LGLogic/LGLogic.cpp

$(IntermediateDirectory)/LGLogic_LGLogic.cpp$(PreprocessSuffix): ./LGLogic/LGLogic.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LGLogic_LGLogic.cpp$(PreprocessSuffix) ./LGLogic/LGLogic.cpp

$(IntermediateDirectory)/LGLogic_LSServerManager.cpp$(ObjectSuffix): ./LGLogic/LSServerManager.cpp $(IntermediateDirectory)/LGLogic_LSServerManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/LGLogic/LGLogic/LSServerManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LGLogic_LSServerManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LGLogic_LSServerManager.cpp$(DependSuffix): ./LGLogic/LSServerManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LGLogic_LSServerManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LGLogic_LSServerManager.cpp$(DependSuffix) -MM ./LGLogic/LSServerManager.cpp

$(IntermediateDirectory)/LGLogic_LSServerManager.cpp$(PreprocessSuffix): ./LGLogic/LSServerManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LGLogic_LSServerManager.cpp$(PreprocessSuffix) ./LGLogic/LSServerManager.cpp

$(IntermediateDirectory)/LGLogic_ListManager.cpp$(ObjectSuffix): ./LGLogic/ListManager.cpp $(IntermediateDirectory)/LGLogic_ListManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/LGLogic/LGLogic/ListManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LGLogic_ListManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LGLogic_ListManager.cpp$(DependSuffix): ./LGLogic/ListManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LGLogic_ListManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LGLogic_ListManager.cpp$(DependSuffix) -MM ./LGLogic/ListManager.cpp

$(IntermediateDirectory)/LGLogic_ListManager.cpp$(PreprocessSuffix): ./LGLogic/ListManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LGLogic_ListManager.cpp$(PreprocessSuffix) ./LGLogic/ListManager.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(WorkspacePath)/Linux_Bin/Temp/


