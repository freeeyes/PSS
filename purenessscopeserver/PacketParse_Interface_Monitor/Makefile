##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=PacketParse_Interface_Monitor
ConfigurationName      :=Release
WorkspacePath          :=../../Build/Linux
ProjectPath            :=../purenessscopeserver/PacketParse_Interface_Monitor
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
ObjectsFileList        :="PacketParse_Interface_Monitor.txt"
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
Objects0=$(IntermediateDirectory)/PacketParse_Interface_Monitor_PacketParse_Interface_Monitor.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "../../Build/Linux/.build-release"
	@echo rebuilt > "../../Build/Linux/.build-release/PacketParse_Interface_Monitor"

MakeIntermediateDirs:
	@test -d $(WorkspacePath)/Temp || $(MakeDirCommand) $(WorkspacePath)/Temp


$(IntermediateDirectory)/.d:
	@test -d $(WorkspacePath)/Temp || $(MakeDirCommand) $(WorkspacePath)/Temp

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/PacketParse_Interface_Monitor_PacketParse_Interface_Monitor.cpp$(ObjectSuffix): PacketParse_Interface_Monitor/PacketParse_Interface_Monitor.cpp $(IntermediateDirectory)/PacketParse_Interface_Monitor_PacketParse_Interface_Monitor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./PacketParse_Interface_Monitor/PacketParse_Interface_Monitor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PacketParse_Interface_Monitor_PacketParse_Interface_Monitor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PacketParse_Interface_Monitor_PacketParse_Interface_Monitor.cpp$(DependSuffix): PacketParse_Interface_Monitor/PacketParse_Interface_Monitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PacketParse_Interface_Monitor_PacketParse_Interface_Monitor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PacketParse_Interface_Monitor_PacketParse_Interface_Monitor.cpp$(DependSuffix) -MM PacketParse_Interface_Monitor/PacketParse_Interface_Monitor.cpp

$(IntermediateDirectory)/PacketParse_Interface_Monitor_PacketParse_Interface_Monitor.cpp$(PreprocessSuffix): PacketParse_Interface_Monitor/PacketParse_Interface_Monitor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PacketParse_Interface_Monitor_PacketParse_Interface_Monitor.cpp$(PreprocessSuffix) PacketParse_Interface_Monitor/PacketParse_Interface_Monitor.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(WorkspacePath)/Temp/


