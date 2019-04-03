##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=TestTCPPost
ConfigurationName      :=Release
WorkspacePath          :=/data/develop/Lib/Network/PSS-master/Build/Linux
ProjectPath            :=/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/TestTCPPost
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
ObjectsFileList        :="TestTCPPost.txt"
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
Objects0=$(IntermediateDirectory)/TestTCPPost_BaseCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/TestTCPPost_TestTCPPost.cpp$(ObjectSuffix) $(IntermediateDirectory)/TestTCPPost_ProxyManager.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/data/develop/Lib/Network/PSS-master/Build/Linux/.build-release/TestTCPPost"

MakeIntermediateDirs:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp


$(IntermediateDirectory)/.d:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/TestTCPPost_BaseCommand.cpp$(ObjectSuffix): ./TestTCPPost/BaseCommand.cpp $(IntermediateDirectory)/TestTCPPost_BaseCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/TestTCPPost/TestTCPPost/BaseCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TestTCPPost_BaseCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TestTCPPost_BaseCommand.cpp$(DependSuffix): ./TestTCPPost/BaseCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TestTCPPost_BaseCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TestTCPPost_BaseCommand.cpp$(DependSuffix) -MM ./TestTCPPost/BaseCommand.cpp

$(IntermediateDirectory)/TestTCPPost_BaseCommand.cpp$(PreprocessSuffix): ./TestTCPPost/BaseCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TestTCPPost_BaseCommand.cpp$(PreprocessSuffix) ./TestTCPPost/BaseCommand.cpp

$(IntermediateDirectory)/TestTCPPost_TestTCPPost.cpp$(ObjectSuffix): ./TestTCPPost/TestTCPPost.cpp $(IntermediateDirectory)/TestTCPPost_TestTCPPost.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/TestTCPPost/TestTCPPost/TestTCPPost.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TestTCPPost_TestTCPPost.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TestTCPPost_TestTCPPost.cpp$(DependSuffix): ./TestTCPPost/TestTCPPost.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TestTCPPost_TestTCPPost.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TestTCPPost_TestTCPPost.cpp$(DependSuffix) -MM ./TestTCPPost/TestTCPPost.cpp

$(IntermediateDirectory)/TestTCPPost_TestTCPPost.cpp$(PreprocessSuffix): ./TestTCPPost/TestTCPPost.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TestTCPPost_TestTCPPost.cpp$(PreprocessSuffix) ./TestTCPPost/TestTCPPost.cpp

$(IntermediateDirectory)/TestTCPPost_ProxyManager.cpp$(ObjectSuffix): ./TestTCPPost/ProxyManager.cpp $(IntermediateDirectory)/TestTCPPost_ProxyManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/TestTCPPost/TestTCPPost/ProxyManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TestTCPPost_ProxyManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TestTCPPost_ProxyManager.cpp$(DependSuffix): ./TestTCPPost/ProxyManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TestTCPPost_ProxyManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TestTCPPost_ProxyManager.cpp$(DependSuffix) -MM ./TestTCPPost/ProxyManager.cpp

$(IntermediateDirectory)/TestTCPPost_ProxyManager.cpp$(PreprocessSuffix): ./TestTCPPost/ProxyManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TestTCPPost_ProxyManager.cpp$(PreprocessSuffix) ./TestTCPPost/ProxyManager.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(WorkspacePath)/Linux_Bin/Temp/


