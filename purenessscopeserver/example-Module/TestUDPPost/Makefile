##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=TestUDPPost
ConfigurationName      :=Release
WorkspacePath          :=../../../Build/Linux
ProjectPath            :=../../purenessscopeserver/example-Module/TestUDPPost
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
ObjectsFileList        :="TestUDPPost.txt"
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
Objects0=$(IntermediateDirectory)/TestUDPPost_BaseCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/TestUDPPost_TestUDPPost.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "../../../Build/Linux/.build-release/TestUDPPost"

MakeIntermediateDirs:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp


$(IntermediateDirectory)/.d:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/TestUDPPost_BaseCommand.cpp$(ObjectSuffix): ./TestUDPPost/BaseCommand.cpp $(IntermediateDirectory)/TestUDPPost_BaseCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./TestUDPPost/BaseCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TestUDPPost_BaseCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TestUDPPost_BaseCommand.cpp$(DependSuffix): ./TestUDPPost/BaseCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TestUDPPost_BaseCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TestUDPPost_BaseCommand.cpp$(DependSuffix) -MM ./TestUDPPost/BaseCommand.cpp

$(IntermediateDirectory)/TestUDPPost_BaseCommand.cpp$(PreprocessSuffix): ./TestUDPPost/BaseCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TestUDPPost_BaseCommand.cpp$(PreprocessSuffix) ./TestUDPPost/BaseCommand.cpp

$(IntermediateDirectory)/TestUDPPost_TestUDPPost.cpp$(ObjectSuffix): ./TestUDPPost/TestUDPPost.cpp $(IntermediateDirectory)/TestUDPPost_TestUDPPost.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./TestUDPPost/TestUDPPost.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TestUDPPost_TestUDPPost.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TestUDPPost_TestUDPPost.cpp$(DependSuffix): ./TestUDPPost/TestUDPPost.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TestUDPPost_TestUDPPost.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TestUDPPost_TestUDPPost.cpp$(DependSuffix) -MM ./TestUDPPost/TestUDPPost.cpp

$(IntermediateDirectory)/TestUDPPost_TestUDPPost.cpp$(PreprocessSuffix): ./TestUDPPost/TestUDPPost.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TestUDPPost_TestUDPPost.cpp$(PreprocessSuffix) ./TestUDPPost/TestUDPPost.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(WorkspacePath)/Linux_Bin/Temp/


