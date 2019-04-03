##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=FtpTest
ConfigurationName      :=Release
WorkspacePath          :=../../../Build/Linux
ProjectPath            :=../../purenessscopeserver/example-Module/FtpTest
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
ObjectsFileList        :="FtpTest.txt"
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
Objects0=$(IntermediateDirectory)/FtpTest_BaseCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/FtpTest_DirView.cpp$(ObjectSuffix) $(IntermediateDirectory)/FtpTest_FtpTest.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "../../../Build/Linux/.build-release/FtpTest"

MakeIntermediateDirs:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp


$(IntermediateDirectory)/.d:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/FtpTest_BaseCommand.cpp$(ObjectSuffix): ./FtpTest/BaseCommand.cpp $(IntermediateDirectory)/FtpTest_BaseCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./FtpTest/BaseCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FtpTest_BaseCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FtpTest_BaseCommand.cpp$(DependSuffix): ./FtpTest/BaseCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FtpTest_BaseCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FtpTest_BaseCommand.cpp$(DependSuffix) -MM ./FtpTest/BaseCommand.cpp

$(IntermediateDirectory)/FtpTest_BaseCommand.cpp$(PreprocessSuffix): ./FtpTest/BaseCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FtpTest_BaseCommand.cpp$(PreprocessSuffix) ./FtpTest/BaseCommand.cpp

$(IntermediateDirectory)/FtpTest_DirView.cpp$(ObjectSuffix): ./FtpTest/DirView.cpp $(IntermediateDirectory)/FtpTest_DirView.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./FtpTest/DirView.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FtpTest_DirView.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FtpTest_DirView.cpp$(DependSuffix): ./FtpTest/DirView.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FtpTest_DirView.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FtpTest_DirView.cpp$(DependSuffix) -MM ./FtpTest/DirView.cpp

$(IntermediateDirectory)/FtpTest_DirView.cpp$(PreprocessSuffix): ./FtpTest/DirView.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FtpTest_DirView.cpp$(PreprocessSuffix) ./FtpTest/DirView.cpp

$(IntermediateDirectory)/FtpTest_FtpTest.cpp$(ObjectSuffix): ./FtpTest/FtpTest.cpp $(IntermediateDirectory)/FtpTest_FtpTest.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./FtpTest/FtpTest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FtpTest_FtpTest.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FtpTest_FtpTest.cpp$(DependSuffix): ./FtpTest/FtpTest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FtpTest_FtpTest.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FtpTest_FtpTest.cpp$(DependSuffix) -MM ./FtpTest/FtpTest.cpp

$(IntermediateDirectory)/FtpTest_FtpTest.cpp$(PreprocessSuffix): ./FtpTest/FtpTest.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FtpTest_FtpTest.cpp$(PreprocessSuffix) ./FtpTest/FtpTest.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(WorkspacePath)/Linux_Bin/Temp/


