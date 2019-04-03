##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=XMLParse
ConfigurationName      :=Release
WorkspacePath          :=/data/develop/Lib/Network/PSS-master/Build/Linux
ProjectPath            :=/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/XMLParse
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
ObjectsFileList        :="XMLParse.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../../../purenessscopeserver/Common $(IncludeSwitch)../../../ThirdLib/include/TinyXML $(IncludeSwitch)../../../ThirdLib/include 
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
Objects0=$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinystr.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix) $(IntermediateDirectory)/XMLParse_XMLParse.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_ThirdLib_include_XmlOpeation.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix) $(IntermediateDirectory)/XMLParse_BaseCommand.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/data/develop/Lib/Network/PSS-master/Build/Linux/.build-release/XMLParse"

MakeIntermediateDirs:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp


$(IntermediateDirectory)/.d:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinystr.cpp$(ObjectSuffix): ../../../ThirdLib/include/TinyXML/tinystr.cpp $(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinystr.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/ThirdLib/include/TinyXML/tinystr.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinystr.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinystr.cpp$(DependSuffix): ../../../ThirdLib/include/TinyXML/tinystr.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinystr.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinystr.cpp$(DependSuffix) -MM ../../../ThirdLib/include/TinyXML/tinystr.cpp

$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinystr.cpp$(PreprocessSuffix): ../../../ThirdLib/include/TinyXML/tinystr.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinystr.cpp$(PreprocessSuffix) ../../../ThirdLib/include/TinyXML/tinystr.cpp

$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(ObjectSuffix): ../../../ThirdLib/include/TinyXML/tinyxml.cpp $(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/ThirdLib/include/TinyXML/tinyxml.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(DependSuffix): ../../../ThirdLib/include/TinyXML/tinyxml.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(DependSuffix) -MM ../../../ThirdLib/include/TinyXML/tinyxml.cpp

$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(PreprocessSuffix): ../../../ThirdLib/include/TinyXML/tinyxml.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(PreprocessSuffix) ../../../ThirdLib/include/TinyXML/tinyxml.cpp

$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix): ../../../ThirdLib/include/TinyXML/tinyxmlerror.cpp $(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/ThirdLib/include/TinyXML/tinyxmlerror.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(DependSuffix): ../../../ThirdLib/include/TinyXML/tinyxmlerror.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(DependSuffix) -MM ../../../ThirdLib/include/TinyXML/tinyxmlerror.cpp

$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(PreprocessSuffix): ../../../ThirdLib/include/TinyXML/tinyxmlerror.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(PreprocessSuffix) ../../../ThirdLib/include/TinyXML/tinyxmlerror.cpp

$(IntermediateDirectory)/XMLParse_XMLParse.cpp$(ObjectSuffix): ./XMLParse/XMLParse.cpp $(IntermediateDirectory)/XMLParse_XMLParse.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/XMLParse/XMLParse/XMLParse.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/XMLParse_XMLParse.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/XMLParse_XMLParse.cpp$(DependSuffix): ./XMLParse/XMLParse.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/XMLParse_XMLParse.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/XMLParse_XMLParse.cpp$(DependSuffix) -MM ./XMLParse/XMLParse.cpp

$(IntermediateDirectory)/XMLParse_XMLParse.cpp$(PreprocessSuffix): ./XMLParse/XMLParse.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/XMLParse_XMLParse.cpp$(PreprocessSuffix) ./XMLParse/XMLParse.cpp

$(IntermediateDirectory)/up_up_up_ThirdLib_include_XmlOpeation.cpp$(ObjectSuffix): ../../../ThirdLib/include/XmlOpeation.cpp $(IntermediateDirectory)/up_up_up_ThirdLib_include_XmlOpeation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/ThirdLib/include/XmlOpeation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_ThirdLib_include_XmlOpeation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_ThirdLib_include_XmlOpeation.cpp$(DependSuffix): ../../../ThirdLib/include/XmlOpeation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_ThirdLib_include_XmlOpeation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_ThirdLib_include_XmlOpeation.cpp$(DependSuffix) -MM ../../../ThirdLib/include/XmlOpeation.cpp

$(IntermediateDirectory)/up_up_up_ThirdLib_include_XmlOpeation.cpp$(PreprocessSuffix): ../../../ThirdLib/include/XmlOpeation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_ThirdLib_include_XmlOpeation.cpp$(PreprocessSuffix) ../../../ThirdLib/include/XmlOpeation.cpp

$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix): ../../../ThirdLib/include/TinyXML/tinyxmlparser.cpp $(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/ThirdLib/include/TinyXML/tinyxmlparser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(DependSuffix): ../../../ThirdLib/include/TinyXML/tinyxmlparser.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(DependSuffix) -MM ../../../ThirdLib/include/TinyXML/tinyxmlparser.cpp

$(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(PreprocessSuffix): ../../../ThirdLib/include/TinyXML/tinyxmlparser.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(PreprocessSuffix) ../../../ThirdLib/include/TinyXML/tinyxmlparser.cpp

$(IntermediateDirectory)/XMLParse_BaseCommand.cpp$(ObjectSuffix): ./XMLParse/BaseCommand.cpp $(IntermediateDirectory)/XMLParse_BaseCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/XMLParse/XMLParse/BaseCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/XMLParse_BaseCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/XMLParse_BaseCommand.cpp$(DependSuffix): ./XMLParse/BaseCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/XMLParse_BaseCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/XMLParse_BaseCommand.cpp$(DependSuffix) -MM ./XMLParse/BaseCommand.cpp

$(IntermediateDirectory)/XMLParse_BaseCommand.cpp$(PreprocessSuffix): ./XMLParse/BaseCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/XMLParse_BaseCommand.cpp$(PreprocessSuffix) ./XMLParse/BaseCommand.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(WorkspacePath)/Linux_Bin/Temp/


