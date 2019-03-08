##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=XMLParse
ConfigurationName      :=Debug
WorkspacePath          :=/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite
ProjectPath            :=/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=fox-mulder
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
OutputFile             :=bin/Debug/libXMLParse
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="XMLParse.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -O0
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../../purenessscopeserver/Common $(IncludeSwitch)../../../Base/include/TinyXML 
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
CXXFLAGS :=  -fPIC -g -Wall -fexceptions  $(Preprocessors)
CFLAGS   :=  -fPIC -g -Wall -fexceptions  $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinystr.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxml.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_Base_include_XmlOpeation.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_purenessscopeserver_example-Module_XMLParse_XMLParse.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_purenessscopeserver_example-Module_XMLParse_BaseCommand.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite/.build-debug"
	@echo rebuilt > "/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite/.build-debug/XMLParse"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinystr.cpp$(ObjectSuffix): ../../../Base/include/TinyXML/tinystr.cpp $(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinystr.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/include/TinyXML/tinystr.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinystr.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinystr.cpp$(DependSuffix): ../../../Base/include/TinyXML/tinystr.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinystr.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinystr.cpp$(DependSuffix) -MM ../../../Base/include/TinyXML/tinystr.cpp

$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinystr.cpp$(PreprocessSuffix): ../../../Base/include/TinyXML/tinystr.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinystr.cpp$(PreprocessSuffix) ../../../Base/include/TinyXML/tinystr.cpp

$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxml.cpp$(ObjectSuffix): ../../../Base/include/TinyXML/tinyxml.cpp $(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxml.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/include/TinyXML/tinyxml.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxml.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxml.cpp$(DependSuffix): ../../../Base/include/TinyXML/tinyxml.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxml.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxml.cpp$(DependSuffix) -MM ../../../Base/include/TinyXML/tinyxml.cpp

$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxml.cpp$(PreprocessSuffix): ../../../Base/include/TinyXML/tinyxml.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxml.cpp$(PreprocessSuffix) ../../../Base/include/TinyXML/tinyxml.cpp

$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix): ../../../Base/include/TinyXML/tinyxmlerror.cpp $(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxmlerror.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/include/TinyXML/tinyxmlerror.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxmlerror.cpp$(DependSuffix): ../../../Base/include/TinyXML/tinyxmlerror.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxmlerror.cpp$(DependSuffix) -MM ../../../Base/include/TinyXML/tinyxmlerror.cpp

$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxmlerror.cpp$(PreprocessSuffix): ../../../Base/include/TinyXML/tinyxmlerror.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxmlerror.cpp$(PreprocessSuffix) ../../../Base/include/TinyXML/tinyxmlerror.cpp

$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix): ../../../Base/include/TinyXML/tinyxmlparser.cpp $(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxmlparser.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/include/TinyXML/tinyxmlparser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxmlparser.cpp$(DependSuffix): ../../../Base/include/TinyXML/tinyxmlparser.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxmlparser.cpp$(DependSuffix) -MM ../../../Base/include/TinyXML/tinyxmlparser.cpp

$(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxmlparser.cpp$(PreprocessSuffix): ../../../Base/include/TinyXML/tinyxmlparser.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_Base_include_TinyXML_tinyxmlparser.cpp$(PreprocessSuffix) ../../../Base/include/TinyXML/tinyxmlparser.cpp

$(IntermediateDirectory)/up_up_up_Base_include_XmlOpeation.cpp$(ObjectSuffix): ../../../Base/include/XmlOpeation.cpp $(IntermediateDirectory)/up_up_up_Base_include_XmlOpeation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/include/XmlOpeation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_Base_include_XmlOpeation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_Base_include_XmlOpeation.cpp$(DependSuffix): ../../../Base/include/XmlOpeation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_Base_include_XmlOpeation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_Base_include_XmlOpeation.cpp$(DependSuffix) -MM ../../../Base/include/XmlOpeation.cpp

$(IntermediateDirectory)/up_up_up_Base_include_XmlOpeation.cpp$(PreprocessSuffix): ../../../Base/include/XmlOpeation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_Base_include_XmlOpeation.cpp$(PreprocessSuffix) ../../../Base/include/XmlOpeation.cpp

$(IntermediateDirectory)/up_up_up_purenessscopeserver_example-Module_XMLParse_XMLParse.cpp$(ObjectSuffix): ../../../purenessscopeserver/example-Module/XMLParse/XMLParse.cpp $(IntermediateDirectory)/up_up_up_purenessscopeserver_example-Module_XMLParse_XMLParse.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/XMLParse/XMLParse.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_purenessscopeserver_example-Module_XMLParse_XMLParse.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_purenessscopeserver_example-Module_XMLParse_XMLParse.cpp$(DependSuffix): ../../../purenessscopeserver/example-Module/XMLParse/XMLParse.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_purenessscopeserver_example-Module_XMLParse_XMLParse.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_purenessscopeserver_example-Module_XMLParse_XMLParse.cpp$(DependSuffix) -MM ../../../purenessscopeserver/example-Module/XMLParse/XMLParse.cpp

$(IntermediateDirectory)/up_up_up_purenessscopeserver_example-Module_XMLParse_XMLParse.cpp$(PreprocessSuffix): ../../../purenessscopeserver/example-Module/XMLParse/XMLParse.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_purenessscopeserver_example-Module_XMLParse_XMLParse.cpp$(PreprocessSuffix) ../../../purenessscopeserver/example-Module/XMLParse/XMLParse.cpp

$(IntermediateDirectory)/up_up_up_purenessscopeserver_example-Module_XMLParse_BaseCommand.cpp$(ObjectSuffix): ../../../purenessscopeserver/example-Module/XMLParse/BaseCommand.cpp $(IntermediateDirectory)/up_up_up_purenessscopeserver_example-Module_XMLParse_BaseCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/XMLParse/BaseCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_purenessscopeserver_example-Module_XMLParse_BaseCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_purenessscopeserver_example-Module_XMLParse_BaseCommand.cpp$(DependSuffix): ../../../purenessscopeserver/example-Module/XMLParse/BaseCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_purenessscopeserver_example-Module_XMLParse_BaseCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_purenessscopeserver_example-Module_XMLParse_BaseCommand.cpp$(DependSuffix) -MM ../../../purenessscopeserver/example-Module/XMLParse/BaseCommand.cpp

$(IntermediateDirectory)/up_up_up_purenessscopeserver_example-Module_XMLParse_BaseCommand.cpp$(PreprocessSuffix): ../../../purenessscopeserver/example-Module/XMLParse/BaseCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_purenessscopeserver_example-Module_XMLParse_BaseCommand.cpp$(PreprocessSuffix) ../../../purenessscopeserver/example-Module/XMLParse/BaseCommand.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


