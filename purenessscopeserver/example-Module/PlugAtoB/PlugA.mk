##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=PlugA
ConfigurationName      :=Release
WorkspacePath          :=/data/develop/Lib/Network/PSS-master/Build/Linux
ProjectPath            :=/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/PlugAtoB
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
ObjectsFileList        :="PlugA.txt"
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
Objects0=$(IntermediateDirectory)/PlugA_BaseCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/PlugA_PlugA.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/data/develop/Lib/Network/PSS-master/Build/Linux/.build-release/PlugA"

MakeIntermediateDirs:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp


$(IntermediateDirectory)/.d:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/PlugA_BaseCommand.cpp$(ObjectSuffix): ./PlugA/BaseCommand.cpp $(IntermediateDirectory)/PlugA_BaseCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/PlugAtoB/PlugA/BaseCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PlugA_BaseCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PlugA_BaseCommand.cpp$(DependSuffix): ./PlugA/BaseCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PlugA_BaseCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PlugA_BaseCommand.cpp$(DependSuffix) -MM ./PlugA/BaseCommand.cpp

$(IntermediateDirectory)/PlugA_BaseCommand.cpp$(PreprocessSuffix): ./PlugA/BaseCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PlugA_BaseCommand.cpp$(PreprocessSuffix) ./PlugA/BaseCommand.cpp

$(IntermediateDirectory)/PlugA_PlugA.cpp$(ObjectSuffix): ./PlugA/PlugA.cpp $(IntermediateDirectory)/PlugA_PlugA.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/PlugAtoB/PlugA/PlugA.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PlugA_PlugA.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PlugA_PlugA.cpp$(DependSuffix): ./PlugA/PlugA.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PlugA_PlugA.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PlugA_PlugA.cpp$(DependSuffix) -MM ./PlugA/PlugA.cpp

$(IntermediateDirectory)/PlugA_PlugA.cpp$(PreprocessSuffix): ./PlugA/PlugA.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PlugA_PlugA.cpp$(PreprocessSuffix) ./PlugA/PlugA.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(WorkspacePath)/Linux_Bin/Temp/


