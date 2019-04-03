##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=MonitorServer
ConfigurationName      :=Release
WorkspacePath          :=/data/develop/Lib/Network/PSS-master/Build/Linux
ProjectPath            :=/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/MonitorServer
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
ObjectsFileList        :="MonitorServer.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../../purenessscopeserver/Common $(IncludeSwitch)../../../ThirdLib/include/EncodeConvert $(IncludeSwitch)../../../ThirdLib/include/TinyXML 
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
Objects0=$(IntermediateDirectory)/MonitorServer_DataManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/MonitorServer_MonitorServer.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(ObjectSuffix) $(IntermediateDirectory)/MonitorServer_TimeEvent.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_ThirdLib_include_EncodeConvert_GBKtoUTF8.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinystr.cpp$(ObjectSuffix) $(IntermediateDirectory)/MonitorServer_MonitorCommand.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/data/develop/Lib/Network/PSS-master/Build/Linux/.build-release/MonitorServer"

MakeIntermediateDirs:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp


$(IntermediateDirectory)/.d:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/MonitorServer_DataManager.cpp$(ObjectSuffix): ./MonitorServer/DataManager.cpp $(IntermediateDirectory)/MonitorServer_DataManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/MonitorServer/MonitorServer/DataManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MonitorServer_DataManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MonitorServer_DataManager.cpp$(DependSuffix): ./MonitorServer/DataManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MonitorServer_DataManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MonitorServer_DataManager.cpp$(DependSuffix) -MM ./MonitorServer/DataManager.cpp

$(IntermediateDirectory)/MonitorServer_DataManager.cpp$(PreprocessSuffix): ./MonitorServer/DataManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MonitorServer_DataManager.cpp$(PreprocessSuffix) ./MonitorServer/DataManager.cpp

$(IntermediateDirectory)/MonitorServer_MonitorServer.cpp$(ObjectSuffix): ./MonitorServer/MonitorServer.cpp $(IntermediateDirectory)/MonitorServer_MonitorServer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/MonitorServer/MonitorServer/MonitorServer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MonitorServer_MonitorServer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MonitorServer_MonitorServer.cpp$(DependSuffix): ./MonitorServer/MonitorServer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MonitorServer_MonitorServer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MonitorServer_MonitorServer.cpp$(DependSuffix) -MM ./MonitorServer/MonitorServer.cpp

$(IntermediateDirectory)/MonitorServer_MonitorServer.cpp$(PreprocessSuffix): ./MonitorServer/MonitorServer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MonitorServer_MonitorServer.cpp$(PreprocessSuffix) ./MonitorServer/MonitorServer.cpp

$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(ObjectSuffix): ../../../ThirdLib/include/TinyXML/tinyxml.cpp $(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/ThirdLib/include/TinyXML/tinyxml.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(DependSuffix): ../../../ThirdLib/include/TinyXML/tinyxml.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(DependSuffix) -MM ../../../ThirdLib/include/TinyXML/tinyxml.cpp

$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(PreprocessSuffix): ../../../ThirdLib/include/TinyXML/tinyxml.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(PreprocessSuffix) ../../../ThirdLib/include/TinyXML/tinyxml.cpp

$(IntermediateDirectory)/MonitorServer_TimeEvent.cpp$(ObjectSuffix): ./MonitorServer/TimeEvent.cpp $(IntermediateDirectory)/MonitorServer_TimeEvent.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/MonitorServer/MonitorServer/TimeEvent.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MonitorServer_TimeEvent.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MonitorServer_TimeEvent.cpp$(DependSuffix): ./MonitorServer/TimeEvent.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MonitorServer_TimeEvent.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MonitorServer_TimeEvent.cpp$(DependSuffix) -MM ./MonitorServer/TimeEvent.cpp

$(IntermediateDirectory)/MonitorServer_TimeEvent.cpp$(PreprocessSuffix): ./MonitorServer/TimeEvent.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MonitorServer_TimeEvent.cpp$(PreprocessSuffix) ./MonitorServer/TimeEvent.cpp

$(IntermediateDirectory)/up_up_up_ThirdLib_include_EncodeConvert_GBKtoUTF8.cpp$(ObjectSuffix): ../../../ThirdLib/include/EncodeConvert/GBKtoUTF8.cpp $(IntermediateDirectory)/up_up_up_ThirdLib_include_EncodeConvert_GBKtoUTF8.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/ThirdLib/include/EncodeConvert/GBKtoUTF8.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_ThirdLib_include_EncodeConvert_GBKtoUTF8.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_ThirdLib_include_EncodeConvert_GBKtoUTF8.cpp$(DependSuffix): ../../../ThirdLib/include/EncodeConvert/GBKtoUTF8.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_ThirdLib_include_EncodeConvert_GBKtoUTF8.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_ThirdLib_include_EncodeConvert_GBKtoUTF8.cpp$(DependSuffix) -MM ../../../ThirdLib/include/EncodeConvert/GBKtoUTF8.cpp

$(IntermediateDirectory)/up_up_up_ThirdLib_include_EncodeConvert_GBKtoUTF8.cpp$(PreprocessSuffix): ../../../ThirdLib/include/EncodeConvert/GBKtoUTF8.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_ThirdLib_include_EncodeConvert_GBKtoUTF8.cpp$(PreprocessSuffix) ../../../ThirdLib/include/EncodeConvert/GBKtoUTF8.cpp

$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix): ../../../ThirdLib/include/TinyXML/tinyxmlerror.cpp $(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/ThirdLib/include/TinyXML/tinyxmlerror.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(DependSuffix): ../../../ThirdLib/include/TinyXML/tinyxmlerror.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(DependSuffix) -MM ../../../ThirdLib/include/TinyXML/tinyxmlerror.cpp

$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(PreprocessSuffix): ../../../ThirdLib/include/TinyXML/tinyxmlerror.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(PreprocessSuffix) ../../../ThirdLib/include/TinyXML/tinyxmlerror.cpp

$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix): ../../../ThirdLib/include/TinyXML/tinyxmlparser.cpp $(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/ThirdLib/include/TinyXML/tinyxmlparser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(DependSuffix): ../../../ThirdLib/include/TinyXML/tinyxmlparser.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(DependSuffix) -MM ../../../ThirdLib/include/TinyXML/tinyxmlparser.cpp

$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(PreprocessSuffix): ../../../ThirdLib/include/TinyXML/tinyxmlparser.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(PreprocessSuffix) ../../../ThirdLib/include/TinyXML/tinyxmlparser.cpp

$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinystr.cpp$(ObjectSuffix): ../../../ThirdLib/include/TinyXML/tinystr.cpp $(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinystr.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/ThirdLib/include/TinyXML/tinystr.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinystr.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinystr.cpp$(DependSuffix): ../../../ThirdLib/include/TinyXML/tinystr.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinystr.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinystr.cpp$(DependSuffix) -MM ../../../ThirdLib/include/TinyXML/tinystr.cpp

$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinystr.cpp$(PreprocessSuffix): ../../../ThirdLib/include/TinyXML/tinystr.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinystr.cpp$(PreprocessSuffix) ../../../ThirdLib/include/TinyXML/tinystr.cpp

$(IntermediateDirectory)/MonitorServer_MonitorCommand.cpp$(ObjectSuffix): ./MonitorServer/MonitorCommand.cpp $(IntermediateDirectory)/MonitorServer_MonitorCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/MonitorServer/MonitorServer/MonitorCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MonitorServer_MonitorCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MonitorServer_MonitorCommand.cpp$(DependSuffix): ./MonitorServer/MonitorCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MonitorServer_MonitorCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MonitorServer_MonitorCommand.cpp$(DependSuffix) -MM ./MonitorServer/MonitorCommand.cpp

$(IntermediateDirectory)/MonitorServer_MonitorCommand.cpp$(PreprocessSuffix): ./MonitorServer/MonitorCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MonitorServer_MonitorCommand.cpp$(PreprocessSuffix) ./MonitorServer/MonitorCommand.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(WorkspacePath)/Linux_Bin/Temp/


