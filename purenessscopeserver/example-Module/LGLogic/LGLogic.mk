##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=LGLogic
ConfigurationName      :=Release
WorkspacePath          :=/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite
ProjectPath            :=/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite/LGLogic
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
OutputFile             :=bin/Release/LGLogic
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="LGLogic.txt"
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
Objects0=$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_BaseCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_LGLogic.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_LSServerManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_ListManager.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite/.build-release/LGLogic"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_BaseCommand.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/LGLogic/BaseCommand.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_BaseCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/LGLogic/BaseCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_BaseCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_BaseCommand.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/LGLogic/BaseCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_BaseCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_BaseCommand.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/LGLogic/BaseCommand.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_BaseCommand.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/LGLogic/BaseCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_BaseCommand.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/LGLogic/BaseCommand.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_LGLogic.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/LGLogic/LGLogic.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_LGLogic.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/LGLogic/LGLogic.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_LGLogic.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_LGLogic.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/LGLogic/LGLogic.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_LGLogic.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_LGLogic.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/LGLogic/LGLogic.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_LGLogic.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/LGLogic/LGLogic.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_LGLogic.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/LGLogic/LGLogic.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_LSServerManager.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/LGLogic/LSServerManager.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_LSServerManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/LGLogic/LSServerManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_LSServerManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_LSServerManager.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/LGLogic/LSServerManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_LSServerManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_LSServerManager.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/LGLogic/LSServerManager.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_LSServerManager.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/LGLogic/LSServerManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_LSServerManager.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/LGLogic/LSServerManager.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_ListManager.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/LGLogic/ListManager.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_ListManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/LGLogic/ListManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_ListManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_ListManager.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/LGLogic/ListManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_ListManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_ListManager.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/LGLogic/ListManager.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_ListManager.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/LGLogic/ListManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LGLogic_ListManager.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/LGLogic/ListManager.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


