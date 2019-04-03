##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=Http
ConfigurationName      :=Release
WorkspacePath          :=../../../Build/Linux
ProjectPath            :=../../purenessscopeserver/example-Module/Http
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
ObjectsFileList        :="Http.txt"
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
Objects0=$(IntermediateDirectory)/Http_BaseCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/Http_HttpTest.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "../../../Build/Linux/.build-release/Http"

MakeIntermediateDirs:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp


$(IntermediateDirectory)/.d:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/Http_BaseCommand.cpp$(ObjectSuffix): ./Http/BaseCommand.cpp $(IntermediateDirectory)/Http_BaseCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./Http/BaseCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Http_BaseCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Http_BaseCommand.cpp$(DependSuffix): ./Http/BaseCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Http_BaseCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Http_BaseCommand.cpp$(DependSuffix) -MM ./Http/BaseCommand.cpp

$(IntermediateDirectory)/Http_BaseCommand.cpp$(PreprocessSuffix): ./Http/BaseCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Http_BaseCommand.cpp$(PreprocessSuffix) ./Http/BaseCommand.cpp

$(IntermediateDirectory)/Http_HttpTest.cpp$(ObjectSuffix): ./Http/HttpTest.cpp $(IntermediateDirectory)/Http_HttpTest.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./Http/HttpTest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Http_HttpTest.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Http_HttpTest.cpp$(DependSuffix): ./Http/HttpTest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Http_HttpTest.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Http_HttpTest.cpp$(DependSuffix) -MM ./Http/HttpTest.cpp

$(IntermediateDirectory)/Http_HttpTest.cpp$(PreprocessSuffix): ./Http/HttpTest.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Http_HttpTest.cpp$(PreprocessSuffix) ./Http/HttpTest.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(WorkspacePath)/Linux_Bin/Temp/


