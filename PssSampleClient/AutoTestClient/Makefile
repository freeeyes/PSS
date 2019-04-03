##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=AutoTestClient
ConfigurationName      :=Release
WorkspacePath          :=../../Build/Linux
ProjectPath            :=../PssSampleClient/AutoTestClient
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
OutputFile             :=$(WorkspacePath)/Linux_Bin/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)NDEBUG $(PreprocessorSwitch)_LINUX_PLATFORM_ 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="AutoTestClient.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../../ThirdLib/include $(IncludeSwitch)../../ThirdLib/include/TinyXML $(IncludeSwitch)../../ThirdLib/thirdparty 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)pthread 
ArLibs                 :=  "libpthread.so" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)/usr/lib/ 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -O2 -Wall $(Preprocessors)
CFLAGS   :=  -O2 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/up_up_ThirdLib_thirdparty_sock_wrap.cpp$(ObjectSuffix) $(IntermediateDirectory)/AutoTestClient_AutoTestClient.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_ThirdLib_include_XmlOpeation.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinystr.cpp$(ObjectSuffix) 



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
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d $(WorkspacePath)/Temp || $(MakeDirCommand) $(WorkspacePath)/Temp


$(IntermediateDirectory)/.d:
	@test -d $(WorkspacePath)/Temp || $(MakeDirCommand) $(WorkspacePath)/Temp

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_up_ThirdLib_thirdparty_sock_wrap.cpp$(ObjectSuffix): ../../ThirdLib/thirdparty/sock_wrap.cpp $(IntermediateDirectory)/up_up_ThirdLib_thirdparty_sock_wrap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../../ThirdLib/thirdparty/sock_wrap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_ThirdLib_thirdparty_sock_wrap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_ThirdLib_thirdparty_sock_wrap.cpp$(DependSuffix): ../../ThirdLib/thirdparty/sock_wrap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_ThirdLib_thirdparty_sock_wrap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_ThirdLib_thirdparty_sock_wrap.cpp$(DependSuffix) -MM ../../ThirdLib/thirdparty/sock_wrap.cpp

$(IntermediateDirectory)/up_up_ThirdLib_thirdparty_sock_wrap.cpp$(PreprocessSuffix): ../../ThirdLib/thirdparty/sock_wrap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_ThirdLib_thirdparty_sock_wrap.cpp$(PreprocessSuffix) ../../ThirdLib/thirdparty/sock_wrap.cpp

$(IntermediateDirectory)/AutoTestClient_AutoTestClient.cpp$(ObjectSuffix): ./AutoTestClient/AutoTestClient.cpp $(IntermediateDirectory)/AutoTestClient_AutoTestClient.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./AutoTestClient/AutoTestClient.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/AutoTestClient_AutoTestClient.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/AutoTestClient_AutoTestClient.cpp$(DependSuffix): ./AutoTestClient/AutoTestClient.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/AutoTestClient_AutoTestClient.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/AutoTestClient_AutoTestClient.cpp$(DependSuffix) -MM ./AutoTestClient/AutoTestClient.cpp

$(IntermediateDirectory)/AutoTestClient_AutoTestClient.cpp$(PreprocessSuffix): ./AutoTestClient/AutoTestClient.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/AutoTestClient_AutoTestClient.cpp$(PreprocessSuffix) ./AutoTestClient/AutoTestClient.cpp

$(IntermediateDirectory)/up_up_ThirdLib_include_XmlOpeation.cpp$(ObjectSuffix): ../../ThirdLib/include/XmlOpeation.cpp $(IntermediateDirectory)/up_up_ThirdLib_include_XmlOpeation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../../ThirdLib/include/XmlOpeation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_ThirdLib_include_XmlOpeation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_ThirdLib_include_XmlOpeation.cpp$(DependSuffix): ../../ThirdLib/include/XmlOpeation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_ThirdLib_include_XmlOpeation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_ThirdLib_include_XmlOpeation.cpp$(DependSuffix) -MM ../../ThirdLib/include/XmlOpeation.cpp

$(IntermediateDirectory)/up_up_ThirdLib_include_XmlOpeation.cpp$(PreprocessSuffix): ../../ThirdLib/include/XmlOpeation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_ThirdLib_include_XmlOpeation.cpp$(PreprocessSuffix) ../../ThirdLib/include/XmlOpeation.cpp

$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(ObjectSuffix): ../../ThirdLib/include/TinyXML/tinyxml.cpp $(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../../ThirdLib/include/TinyXML/tinyxml.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(DependSuffix): ../../ThirdLib/include/TinyXML/tinyxml.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(DependSuffix) -MM ../../ThirdLib/include/TinyXML/tinyxml.cpp

$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(PreprocessSuffix): ../../ThirdLib/include/TinyXML/tinyxml.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxml.cpp$(PreprocessSuffix) ../../ThirdLib/include/TinyXML/tinyxml.cpp

$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix): ../../ThirdLib/include/TinyXML/tinyxmlparser.cpp $(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../../ThirdLib/include/TinyXML/tinyxmlparser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(DependSuffix): ../../ThirdLib/include/TinyXML/tinyxmlparser.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(DependSuffix) -MM ../../ThirdLib/include/TinyXML/tinyxmlparser.cpp

$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(PreprocessSuffix): ../../ThirdLib/include/TinyXML/tinyxmlparser.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(PreprocessSuffix) ../../ThirdLib/include/TinyXML/tinyxmlparser.cpp

$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix): ../../ThirdLib/include/TinyXML/tinyxmlerror.cpp $(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../../ThirdLib/include/TinyXML/tinyxmlerror.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(DependSuffix): ../../ThirdLib/include/TinyXML/tinyxmlerror.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(DependSuffix) -MM ../../ThirdLib/include/TinyXML/tinyxmlerror.cpp

$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(PreprocessSuffix): ../../ThirdLib/include/TinyXML/tinyxmlerror.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(PreprocessSuffix) ../../ThirdLib/include/TinyXML/tinyxmlerror.cpp

$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinystr.cpp$(ObjectSuffix): ../../ThirdLib/include/TinyXML/tinystr.cpp $(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinystr.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../../ThirdLib/include/TinyXML/tinystr.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinystr.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinystr.cpp$(DependSuffix): ../../ThirdLib/include/TinyXML/tinystr.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinystr.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinystr.cpp$(DependSuffix) -MM ../../ThirdLib/include/TinyXML/tinystr.cpp

$(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinystr.cpp$(PreprocessSuffix): ../../ThirdLib/include/TinyXML/tinystr.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_ThirdLib_include_TinyXML_tinystr.cpp$(PreprocessSuffix) ../../ThirdLib/include/TinyXML/tinystr.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(WorkspacePath)/Temp/


