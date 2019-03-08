##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=KafkaClient
ConfigurationName      :=Release
WorkspacePath          :=/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite
ProjectPath            :=/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite/KafkaClient
IntermediateDirectory  :=./Release
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
OutputFile             :=bin/Release/KafkaClient
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="KafkaClient.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../../../purenessscopeserver/Common $(IncludeSwitch)/usr/local/include/librdkafka 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)/usr/local/lib/librdkafka++ 
ArLibs                 :=  "/usr/local/lib/librdkafka++.a" 
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
Objects0=$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_BaseCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_KafkaClient.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_kafkaTest.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite/.build-release/KafkaClient"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


$(IntermediateDirectory)/.d:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_BaseCommand.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/KafkaClient/BaseCommand.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_BaseCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/KafkaClient/BaseCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_BaseCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_BaseCommand.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/KafkaClient/BaseCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_BaseCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_BaseCommand.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/KafkaClient/BaseCommand.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_BaseCommand.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/KafkaClient/BaseCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_BaseCommand.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/KafkaClient/BaseCommand.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_KafkaClient.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/KafkaClient/KafkaClient.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_KafkaClient.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/KafkaClient/KafkaClient.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_KafkaClient.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_KafkaClient.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/KafkaClient/KafkaClient.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_KafkaClient.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_KafkaClient.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/KafkaClient/KafkaClient.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_KafkaClient.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/KafkaClient/KafkaClient.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_KafkaClient.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/KafkaClient/KafkaClient.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_kafkaTest.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/KafkaClient/kafkaTest.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_kafkaTest.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/KafkaClient/kafkaTest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_kafkaTest.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_kafkaTest.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/KafkaClient/kafkaTest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_kafkaTest.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_kafkaTest.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/KafkaClient/kafkaTest.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_kafkaTest.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/KafkaClient/kafkaTest.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_KafkaClient_kafkaTest.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/KafkaClient/kafkaTest.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


