##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=TTyTest
ConfigurationName      :=Release
WorkspacePath          :=../../../Build/Linux
ProjectPath            :=../../purenessscopeserver/example-Module/TTyTest
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
Preprocessors          :=$(PreprocessorSwitch)__LINUX__ 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="TTyTest.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../../ThirdLib/include/Common $(IncludeSwitch)../../Common 
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
Objects0=$(IntermediateDirectory)/TTyTest_BaseCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/TTyTest_TTyTest.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "../../../Build/Linux/.build-release/TTyTest"

MakeIntermediateDirs:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp


$(IntermediateDirectory)/.d:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/TTyTest_BaseCommand.cpp$(ObjectSuffix): ./TTyTest/BaseCommand.cpp $(IntermediateDirectory)/TTyTest_BaseCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./TTyTest/BaseCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TTyTest_BaseCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TTyTest_BaseCommand.cpp$(DependSuffix): ./TTyTest/BaseCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TTyTest_BaseCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TTyTest_BaseCommand.cpp$(DependSuffix) -MM ./TTyTest/BaseCommand.cpp

$(IntermediateDirectory)/TTyTest_BaseCommand.cpp$(PreprocessSuffix): ./TTyTest/BaseCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TTyTest_BaseCommand.cpp$(PreprocessSuffix) ./TTyTest/BaseCommand.cpp

$(IntermediateDirectory)/TTyTest_TTyTest.cpp$(ObjectSuffix): ./TTyTest/TTyTest.cpp $(IntermediateDirectory)/TTyTest_TTyTest.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./TTyTest/TTyTest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TTyTest_TTyTest.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TTyTest_TTyTest.cpp$(DependSuffix): ./TTyTest/TTyTest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TTyTest_TTyTest.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TTyTest_TTyTest.cpp$(DependSuffix) -MM ./TTyTest/TTyTest.cpp

$(IntermediateDirectory)/TTyTest_TTyTest.cpp$(PreprocessSuffix): ./TTyTest/TTyTest.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TTyTest_TTyTest.cpp$(PreprocessSuffix) ./TTyTest/TTyTest.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(WorkspacePath)/Linux_Bin/Temp/


