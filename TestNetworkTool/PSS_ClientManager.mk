##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=PSS_ClientManager
ConfigurationName      :=Release
WorkspacePath          :=/data/develop/Lib/Network/PSS-master/Build/Linux
ProjectPath            :=/data/develop/Lib/Network/PSS-master/TestNetworkTool
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
ObjectsFileList        :="PSS_ClientManager.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s $(shell wx-config --debug=no --libs --unicode=yes)
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../ThirdLib/include $(IncludeSwitch)../ThirdLib/include/Lua $(IncludeSwitch)../ThirdLib/src $(IncludeSwitch)../ThirdLib/thirdparty $(IncludeSwitch)../ThirdLib/thirdparty/LuaLib $(IncludeSwitch)../ThirdLib/include/TinyXML 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -O2 -Wall $(shell wx-config --cxxflags --unicode=yes --debug=no) $(Preprocessors)
CFLAGS   :=  -O2 -Wall $(shell wx-config --cxxflags --unicode=yes --debug=no) $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinystr.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_ThirdLib_include_XmlOpeation.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxml.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_ThirdLib_thirdparty_sock_wrap.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_ThirdLib_src_TcpClientConnect.cpp$(ObjectSuffix) $(IntermediateDirectory)/PSS_ClientManager_PSS_ClientManagerApp.cpp$(ObjectSuffix) $(IntermediateDirectory)/PSS_ClientManager_PSS_ClientManagerMain.cpp$(ObjectSuffix) 



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
$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinystr.cpp$(ObjectSuffix): ../ThirdLib/include/TinyXML/tinystr.cpp $(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinystr.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/ThirdLib/include/TinyXML/tinystr.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinystr.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinystr.cpp$(DependSuffix): ../ThirdLib/include/TinyXML/tinystr.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinystr.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinystr.cpp$(DependSuffix) -MM ../ThirdLib/include/TinyXML/tinystr.cpp

$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinystr.cpp$(PreprocessSuffix): ../ThirdLib/include/TinyXML/tinystr.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinystr.cpp$(PreprocessSuffix) ../ThirdLib/include/TinyXML/tinystr.cpp

$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix): ../ThirdLib/include/TinyXML/tinyxmlparser.cpp $(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/ThirdLib/include/TinyXML/tinyxmlparser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(DependSuffix): ../ThirdLib/include/TinyXML/tinyxmlparser.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(DependSuffix) -MM ../ThirdLib/include/TinyXML/tinyxmlparser.cpp

$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(PreprocessSuffix): ../ThirdLib/include/TinyXML/tinyxmlparser.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxmlparser.cpp$(PreprocessSuffix) ../ThirdLib/include/TinyXML/tinyxmlparser.cpp

$(IntermediateDirectory)/up_ThirdLib_include_XmlOpeation.cpp$(ObjectSuffix): ../ThirdLib/include/XmlOpeation.cpp $(IntermediateDirectory)/up_ThirdLib_include_XmlOpeation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/ThirdLib/include/XmlOpeation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_ThirdLib_include_XmlOpeation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_ThirdLib_include_XmlOpeation.cpp$(DependSuffix): ../ThirdLib/include/XmlOpeation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_ThirdLib_include_XmlOpeation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_ThirdLib_include_XmlOpeation.cpp$(DependSuffix) -MM ../ThirdLib/include/XmlOpeation.cpp

$(IntermediateDirectory)/up_ThirdLib_include_XmlOpeation.cpp$(PreprocessSuffix): ../ThirdLib/include/XmlOpeation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_ThirdLib_include_XmlOpeation.cpp$(PreprocessSuffix) ../ThirdLib/include/XmlOpeation.cpp

$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxml.cpp$(ObjectSuffix): ../ThirdLib/include/TinyXML/tinyxml.cpp $(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxml.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/ThirdLib/include/TinyXML/tinyxml.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxml.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxml.cpp$(DependSuffix): ../ThirdLib/include/TinyXML/tinyxml.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxml.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxml.cpp$(DependSuffix) -MM ../ThirdLib/include/TinyXML/tinyxml.cpp

$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxml.cpp$(PreprocessSuffix): ../ThirdLib/include/TinyXML/tinyxml.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxml.cpp$(PreprocessSuffix) ../ThirdLib/include/TinyXML/tinyxml.cpp

$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix): ../ThirdLib/include/TinyXML/tinyxmlerror.cpp $(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/ThirdLib/include/TinyXML/tinyxmlerror.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(DependSuffix): ../ThirdLib/include/TinyXML/tinyxmlerror.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(DependSuffix) -MM ../ThirdLib/include/TinyXML/tinyxmlerror.cpp

$(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(PreprocessSuffix): ../ThirdLib/include/TinyXML/tinyxmlerror.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_ThirdLib_include_TinyXML_tinyxmlerror.cpp$(PreprocessSuffix) ../ThirdLib/include/TinyXML/tinyxmlerror.cpp

$(IntermediateDirectory)/up_ThirdLib_thirdparty_sock_wrap.cpp$(ObjectSuffix): ../ThirdLib/thirdparty/sock_wrap.cpp $(IntermediateDirectory)/up_ThirdLib_thirdparty_sock_wrap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/ThirdLib/thirdparty/sock_wrap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_ThirdLib_thirdparty_sock_wrap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_ThirdLib_thirdparty_sock_wrap.cpp$(DependSuffix): ../ThirdLib/thirdparty/sock_wrap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_ThirdLib_thirdparty_sock_wrap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_ThirdLib_thirdparty_sock_wrap.cpp$(DependSuffix) -MM ../ThirdLib/thirdparty/sock_wrap.cpp

$(IntermediateDirectory)/up_ThirdLib_thirdparty_sock_wrap.cpp$(PreprocessSuffix): ../ThirdLib/thirdparty/sock_wrap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_ThirdLib_thirdparty_sock_wrap.cpp$(PreprocessSuffix) ../ThirdLib/thirdparty/sock_wrap.cpp

$(IntermediateDirectory)/up_ThirdLib_src_TcpClientConnect.cpp$(ObjectSuffix): ../ThirdLib/src/TcpClientConnect.cpp $(IntermediateDirectory)/up_ThirdLib_src_TcpClientConnect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/ThirdLib/src/TcpClientConnect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_ThirdLib_src_TcpClientConnect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_ThirdLib_src_TcpClientConnect.cpp$(DependSuffix): ../ThirdLib/src/TcpClientConnect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_ThirdLib_src_TcpClientConnect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_ThirdLib_src_TcpClientConnect.cpp$(DependSuffix) -MM ../ThirdLib/src/TcpClientConnect.cpp

$(IntermediateDirectory)/up_ThirdLib_src_TcpClientConnect.cpp$(PreprocessSuffix): ../ThirdLib/src/TcpClientConnect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_ThirdLib_src_TcpClientConnect.cpp$(PreprocessSuffix) ../ThirdLib/src/TcpClientConnect.cpp

$(IntermediateDirectory)/PSS_ClientManager_PSS_ClientManagerApp.cpp$(ObjectSuffix): PSS_ClientManager/PSS_ClientManagerApp.cpp $(IntermediateDirectory)/PSS_ClientManager_PSS_ClientManagerApp.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/TestNetworkTool/PSS_ClientManager/PSS_ClientManagerApp.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PSS_ClientManager_PSS_ClientManagerApp.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PSS_ClientManager_PSS_ClientManagerApp.cpp$(DependSuffix): PSS_ClientManager/PSS_ClientManagerApp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PSS_ClientManager_PSS_ClientManagerApp.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PSS_ClientManager_PSS_ClientManagerApp.cpp$(DependSuffix) -MM PSS_ClientManager/PSS_ClientManagerApp.cpp

$(IntermediateDirectory)/PSS_ClientManager_PSS_ClientManagerApp.cpp$(PreprocessSuffix): PSS_ClientManager/PSS_ClientManagerApp.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PSS_ClientManager_PSS_ClientManagerApp.cpp$(PreprocessSuffix) PSS_ClientManager/PSS_ClientManagerApp.cpp

$(IntermediateDirectory)/PSS_ClientManager_PSS_ClientManagerMain.cpp$(ObjectSuffix): PSS_ClientManager/PSS_ClientManagerMain.cpp $(IntermediateDirectory)/PSS_ClientManager_PSS_ClientManagerMain.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/TestNetworkTool/PSS_ClientManager/PSS_ClientManagerMain.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PSS_ClientManager_PSS_ClientManagerMain.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PSS_ClientManager_PSS_ClientManagerMain.cpp$(DependSuffix): PSS_ClientManager/PSS_ClientManagerMain.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PSS_ClientManager_PSS_ClientManagerMain.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PSS_ClientManager_PSS_ClientManagerMain.cpp$(DependSuffix) -MM PSS_ClientManager/PSS_ClientManagerMain.cpp

$(IntermediateDirectory)/PSS_ClientManager_PSS_ClientManagerMain.cpp$(PreprocessSuffix): PSS_ClientManager/PSS_ClientManagerMain.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PSS_ClientManager_PSS_ClientManagerMain.cpp$(PreprocessSuffix) PSS_ClientManager/PSS_ClientManagerMain.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(WorkspacePath)/Temp/


