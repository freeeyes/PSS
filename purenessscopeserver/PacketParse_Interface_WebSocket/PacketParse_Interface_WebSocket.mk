##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=PacketParse_Interface_WebSocket
ConfigurationName      :=Release
WorkspacePath          :=/data/develop/Lib/Network/PSS-master/Build/Linux
ProjectPath            :=/data/develop/Lib/Network/PSS-master/purenessscopeserver/PacketParse_Interface_WebSocket
IntermediateDirectory  :=$(WorkspacePath)/Temp
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
OutputFile             :=$(WorkspacePath)/Linux_Bin/Module/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="PacketParse_Interface_WebSocket.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../ThirdLib/include/Common $(IncludeSwitch)../Common 
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
Objects0=$(IntermediateDirectory)/PacketParse_Interface_WebSocket_PacketParse_Interface.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/data/develop/Lib/Network/PSS-master/Build/Linux/.build-release/PacketParse_Interface_WebSocket"

MakeIntermediateDirs:
	@test -d $(WorkspacePath)/Temp || $(MakeDirCommand) $(WorkspacePath)/Temp


$(IntermediateDirectory)/.d:
	@test -d $(WorkspacePath)/Temp || $(MakeDirCommand) $(WorkspacePath)/Temp

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/PacketParse_Interface_WebSocket_PacketParse_Interface.cpp$(ObjectSuffix): PacketParse_Interface_WebSocket/PacketParse_Interface.cpp $(IntermediateDirectory)/PacketParse_Interface_WebSocket_PacketParse_Interface.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/PacketParse_Interface_WebSocket/PacketParse_Interface_WebSocket/PacketParse_Interface.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PacketParse_Interface_WebSocket_PacketParse_Interface.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PacketParse_Interface_WebSocket_PacketParse_Interface.cpp$(DependSuffix): PacketParse_Interface_WebSocket/PacketParse_Interface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PacketParse_Interface_WebSocket_PacketParse_Interface.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PacketParse_Interface_WebSocket_PacketParse_Interface.cpp$(DependSuffix) -MM PacketParse_Interface_WebSocket/PacketParse_Interface.cpp

$(IntermediateDirectory)/PacketParse_Interface_WebSocket_PacketParse_Interface.cpp$(PreprocessSuffix): PacketParse_Interface_WebSocket/PacketParse_Interface.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PacketParse_Interface_WebSocket_PacketParse_Interface.cpp$(PreprocessSuffix) PacketParse_Interface_WebSocket/PacketParse_Interface.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(WorkspacePath)/Temp/


