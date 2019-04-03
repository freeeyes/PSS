##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=LsPlugIn
ConfigurationName      :=Release
WorkspacePath          :=../../../Build/Linux
ProjectPath            :=../../purenessscopeserver/example-Module/LsPlugIn
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
ObjectsFileList        :="LsPlugIn.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../Common $(IncludeSwitch)./LoginBuffer/Common $(IncludeSwitch)./Common 
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
Objects0=$(IntermediateDirectory)/LsPlugIn_MD5.cpp$(ObjectSuffix) $(IntermediateDirectory)/LsPlugIn_ListManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/LsPlugIn_BaseCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/LsPlugIn_LSLogic.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "../../../Build/Linux/.build-release/LsPlugIn"

MakeIntermediateDirs:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp


$(IntermediateDirectory)/.d:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/LsPlugIn_MD5.cpp$(ObjectSuffix): ./LsPlugIn/MD5.cpp $(IntermediateDirectory)/LsPlugIn_MD5.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./LsPlugIn/MD5.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LsPlugIn_MD5.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LsPlugIn_MD5.cpp$(DependSuffix): ./LsPlugIn/MD5.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LsPlugIn_MD5.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LsPlugIn_MD5.cpp$(DependSuffix) -MM ./LsPlugIn/MD5.cpp

$(IntermediateDirectory)/LsPlugIn_MD5.cpp$(PreprocessSuffix): ./LsPlugIn/MD5.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LsPlugIn_MD5.cpp$(PreprocessSuffix) ./LsPlugIn/MD5.cpp

$(IntermediateDirectory)/LsPlugIn_ListManager.cpp$(ObjectSuffix): ./LsPlugIn/ListManager.cpp $(IntermediateDirectory)/LsPlugIn_ListManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./LsPlugIn/ListManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LsPlugIn_ListManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LsPlugIn_ListManager.cpp$(DependSuffix): ./LsPlugIn/ListManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LsPlugIn_ListManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LsPlugIn_ListManager.cpp$(DependSuffix) -MM ./LsPlugIn/ListManager.cpp

$(IntermediateDirectory)/LsPlugIn_ListManager.cpp$(PreprocessSuffix): ./LsPlugIn/ListManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LsPlugIn_ListManager.cpp$(PreprocessSuffix) ./LsPlugIn/ListManager.cpp

$(IntermediateDirectory)/LsPlugIn_BaseCommand.cpp$(ObjectSuffix): ./LsPlugIn/BaseCommand.cpp $(IntermediateDirectory)/LsPlugIn_BaseCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./LsPlugIn/BaseCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LsPlugIn_BaseCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LsPlugIn_BaseCommand.cpp$(DependSuffix): ./LsPlugIn/BaseCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LsPlugIn_BaseCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LsPlugIn_BaseCommand.cpp$(DependSuffix) -MM ./LsPlugIn/BaseCommand.cpp

$(IntermediateDirectory)/LsPlugIn_BaseCommand.cpp$(PreprocessSuffix): ./LsPlugIn/BaseCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LsPlugIn_BaseCommand.cpp$(PreprocessSuffix) ./LsPlugIn/BaseCommand.cpp

$(IntermediateDirectory)/LsPlugIn_LSLogic.cpp$(ObjectSuffix): ./LsPlugIn/LSLogic.cpp $(IntermediateDirectory)/LsPlugIn_LSLogic.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./LsPlugIn/LSLogic.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LsPlugIn_LSLogic.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LsPlugIn_LSLogic.cpp$(DependSuffix): ./LsPlugIn/LSLogic.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LsPlugIn_LSLogic.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LsPlugIn_LSLogic.cpp$(DependSuffix) -MM ./LsPlugIn/LSLogic.cpp

$(IntermediateDirectory)/LsPlugIn_LSLogic.cpp$(PreprocessSuffix): ./LsPlugIn/LSLogic.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LsPlugIn_LSLogic.cpp$(PreprocessSuffix) ./LsPlugIn/LSLogic.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(WorkspacePath)/Linux_Bin/Temp/


