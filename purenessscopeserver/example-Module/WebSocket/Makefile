##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=WebSocket
ConfigurationName      :=Release
WorkspacePath          :=../../../Build/Linux
ProjectPath            :=../../purenessscopeserver/example-Module/WebSocket
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
ObjectsFileList        :="WebSocket.txt"
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
Objects0=$(IntermediateDirectory)/WebSocket_BaseCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/WebSocket_base64.cpp$(ObjectSuffix) $(IntermediateDirectory)/WebSocket_WebSocketTest.cpp$(ObjectSuffix) $(IntermediateDirectory)/WebSocket_sha1.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "../../../Build/Linux/.build-release/WebSocket"

MakeIntermediateDirs:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp


$(IntermediateDirectory)/.d:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/WebSocket_BaseCommand.cpp$(ObjectSuffix): ./WebSocket/BaseCommand.cpp $(IntermediateDirectory)/WebSocket_BaseCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./WebSocket/BaseCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/WebSocket_BaseCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/WebSocket_BaseCommand.cpp$(DependSuffix): ./WebSocket/BaseCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/WebSocket_BaseCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/WebSocket_BaseCommand.cpp$(DependSuffix) -MM ./WebSocket/BaseCommand.cpp

$(IntermediateDirectory)/WebSocket_BaseCommand.cpp$(PreprocessSuffix): ./WebSocket/BaseCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/WebSocket_BaseCommand.cpp$(PreprocessSuffix) ./WebSocket/BaseCommand.cpp

$(IntermediateDirectory)/WebSocket_base64.cpp$(ObjectSuffix): ./WebSocket/base64.cpp $(IntermediateDirectory)/WebSocket_base64.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./WebSocket/base64.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/WebSocket_base64.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/WebSocket_base64.cpp$(DependSuffix): ./WebSocket/base64.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/WebSocket_base64.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/WebSocket_base64.cpp$(DependSuffix) -MM ./WebSocket/base64.cpp

$(IntermediateDirectory)/WebSocket_base64.cpp$(PreprocessSuffix): ./WebSocket/base64.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/WebSocket_base64.cpp$(PreprocessSuffix) ./WebSocket/base64.cpp

$(IntermediateDirectory)/WebSocket_WebSocketTest.cpp$(ObjectSuffix): ./WebSocket/WebSocketTest.cpp $(IntermediateDirectory)/WebSocket_WebSocketTest.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./WebSocket/WebSocketTest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/WebSocket_WebSocketTest.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/WebSocket_WebSocketTest.cpp$(DependSuffix): ./WebSocket/WebSocketTest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/WebSocket_WebSocketTest.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/WebSocket_WebSocketTest.cpp$(DependSuffix) -MM ./WebSocket/WebSocketTest.cpp

$(IntermediateDirectory)/WebSocket_WebSocketTest.cpp$(PreprocessSuffix): ./WebSocket/WebSocketTest.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/WebSocket_WebSocketTest.cpp$(PreprocessSuffix) ./WebSocket/WebSocketTest.cpp

$(IntermediateDirectory)/WebSocket_sha1.cpp$(ObjectSuffix): ./WebSocket/sha1.cpp $(IntermediateDirectory)/WebSocket_sha1.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./WebSocket/sha1.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/WebSocket_sha1.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/WebSocket_sha1.cpp$(DependSuffix): ./WebSocket/sha1.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/WebSocket_sha1.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/WebSocket_sha1.cpp$(DependSuffix) -MM ./WebSocket/sha1.cpp

$(IntermediateDirectory)/WebSocket_sha1.cpp$(PreprocessSuffix): ./WebSocket/sha1.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/WebSocket_sha1.cpp$(PreprocessSuffix) ./WebSocket/sha1.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(WorkspacePath)/Linux_Bin/Temp/


