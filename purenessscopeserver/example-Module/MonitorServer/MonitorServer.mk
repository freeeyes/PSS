##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=MonitorServer
ConfigurationName      :=Release
WorkspacePath          :=/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite
ProjectPath            :=/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite/MonitorServer
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=fox mulder
Date                   :=2019年03月06日
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
OutputFile             :=bin/Release/libMonitorServer
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="MonitorServer.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../../../purenessscopeserver/Common $(IncludeSwitch)../../../../Base/include/EncodeConvert $(IncludeSwitch)../../../../Base/include/TinyXML 
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
Objects0=$(IntermediateDirectory)/up_up_up_up_Base_include_EncodeConvert_GBKtoUTF8.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_DataManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_TimeEvent.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxml.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinystr.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_MonitorCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_MonitorServer.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite/.build-release"
	@echo rebuilt > "/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite/.build-release/MonitorServer"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_up_up_up_Base_include_EncodeConvert_GBKtoUTF8.cpp$(ObjectSuffix): ../../../../Base/include/EncodeConvert/GBKtoUTF8.cpp $(IntermediateDirectory)/up_up_up_up_Base_include_EncodeConvert_GBKtoUTF8.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/include/EncodeConvert/GBKtoUTF8.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_include_EncodeConvert_GBKtoUTF8.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_include_EncodeConvert_GBKtoUTF8.cpp$(DependSuffix): ../../../../Base/include/EncodeConvert/GBKtoUTF8.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_include_EncodeConvert_GBKtoUTF8.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_include_EncodeConvert_GBKtoUTF8.cpp$(DependSuffix) -MM ../../../../Base/include/EncodeConvert/GBKtoUTF8.cpp

$(IntermediateDirectory)/up_up_up_up_Base_include_EncodeConvert_GBKtoUTF8.cpp$(PreprocessSuffix): ../../../../Base/include/EncodeConvert/GBKtoUTF8.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_include_EncodeConvert_GBKtoUTF8.cpp$(PreprocessSuffix) ../../../../Base/include/EncodeConvert/GBKtoUTF8.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_DataManager.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/MonitorServer/DataManager.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_DataManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/MonitorServer/DataManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_DataManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_DataManager.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/MonitorServer/DataManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_DataManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_DataManager.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/MonitorServer/DataManager.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_DataManager.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/MonitorServer/DataManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_DataManager.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/MonitorServer/DataManager.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_TimeEvent.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/MonitorServer/TimeEvent.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_TimeEvent.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/MonitorServer/TimeEvent.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_TimeEvent.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_TimeEvent.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/MonitorServer/TimeEvent.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_TimeEvent.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_TimeEvent.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/MonitorServer/TimeEvent.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_TimeEvent.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/MonitorServer/TimeEvent.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_TimeEvent.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/MonitorServer/TimeEvent.cpp

$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxml.cpp$(ObjectSuffix): ../../../../Base/include/TinyXML/tinyxml.cpp $(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxml.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/include/TinyXML/tinyxml.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxml.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxml.cpp$(DependSuffix): ../../../../Base/include/TinyXML/tinyxml.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxml.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxml.cpp$(DependSuffix) -MM ../../../../Base/include/TinyXML/tinyxml.cpp

$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxml.cpp$(PreprocessSuffix): ../../../../Base/include/TinyXML/tinyxml.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxml.cpp$(PreprocessSuffix) ../../../../Base/include/TinyXML/tinyxml.cpp

$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix): ../../../../Base/include/TinyXML/tinyxmlerror.cpp $(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxmlerror.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/include/TinyXML/tinyxmlerror.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxmlerror.cpp$(DependSuffix): ../../../../Base/include/TinyXML/tinyxmlerror.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxmlerror.cpp$(DependSuffix) -MM ../../../../Base/include/TinyXML/tinyxmlerror.cpp

$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxmlerror.cpp$(PreprocessSuffix): ../../../../Base/include/TinyXML/tinyxmlerror.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxmlerror.cpp$(PreprocessSuffix) ../../../../Base/include/TinyXML/tinyxmlerror.cpp

$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix): ../../../../Base/include/TinyXML/tinyxmlparser.cpp $(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxmlparser.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/include/TinyXML/tinyxmlparser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxmlparser.cpp$(DependSuffix): ../../../../Base/include/TinyXML/tinyxmlparser.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxmlparser.cpp$(DependSuffix) -MM ../../../../Base/include/TinyXML/tinyxmlparser.cpp

$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxmlparser.cpp$(PreprocessSuffix): ../../../../Base/include/TinyXML/tinyxmlparser.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinyxmlparser.cpp$(PreprocessSuffix) ../../../../Base/include/TinyXML/tinyxmlparser.cpp

$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinystr.cpp$(ObjectSuffix): ../../../../Base/include/TinyXML/tinystr.cpp $(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinystr.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/include/TinyXML/tinystr.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinystr.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinystr.cpp$(DependSuffix): ../../../../Base/include/TinyXML/tinystr.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinystr.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinystr.cpp$(DependSuffix) -MM ../../../../Base/include/TinyXML/tinystr.cpp

$(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinystr.cpp$(PreprocessSuffix): ../../../../Base/include/TinyXML/tinystr.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_include_TinyXML_tinystr.cpp$(PreprocessSuffix) ../../../../Base/include/TinyXML/tinystr.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_MonitorCommand.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/MonitorServer/MonitorCommand.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_MonitorCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/MonitorServer/MonitorCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_MonitorCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_MonitorCommand.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/MonitorServer/MonitorCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_MonitorCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_MonitorCommand.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/MonitorServer/MonitorCommand.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_MonitorCommand.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/MonitorServer/MonitorCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_MonitorCommand.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/MonitorServer/MonitorCommand.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_MonitorServer.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/MonitorServer/MonitorServer.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_MonitorServer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/MonitorServer/MonitorServer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_MonitorServer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_MonitorServer.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/MonitorServer/MonitorServer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_MonitorServer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_MonitorServer.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/MonitorServer/MonitorServer.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_MonitorServer.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/MonitorServer/MonitorServer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_MonitorServer_MonitorServer.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/MonitorServer/MonitorServer.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


