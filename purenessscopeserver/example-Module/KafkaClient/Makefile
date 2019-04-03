##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=KafkaClient
ConfigurationName      :=Release
WorkspacePath          :=../../../Build/Linux
ProjectPath            :=../../purenessscopeserver/example-Module/KafkaClient
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
ObjectsFileList        :="KafkaClient.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../../purenessscopeserver/Common $(IncludeSwitch)/usr/local/include/librdkafka 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)rdkafka++ 
ArLibs                 :=  "librdkafka++.a" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)/usr/local/lib/ 

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
Objects0=$(IntermediateDirectory)/KafkaClient_BaseCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/KafkaClient_kafkaTest.cpp$(ObjectSuffix) $(IntermediateDirectory)/KafkaClient_KafkaClient.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "../../../Build/Linux/.build-release/KafkaClient"

MakeIntermediateDirs:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp


$(IntermediateDirectory)/.d:
	@test -d $(WorkspacePath)/Linux_Bin/Temp || $(MakeDirCommand) $(WorkspacePath)/Linux_Bin/Temp

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/KafkaClient_BaseCommand.cpp$(ObjectSuffix): ./KafkaClient/BaseCommand.cpp $(IntermediateDirectory)/KafkaClient_BaseCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./KafkaClient/BaseCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/KafkaClient_BaseCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/KafkaClient_BaseCommand.cpp$(DependSuffix): ./KafkaClient/BaseCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/KafkaClient_BaseCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/KafkaClient_BaseCommand.cpp$(DependSuffix) -MM ./KafkaClient/BaseCommand.cpp

$(IntermediateDirectory)/KafkaClient_BaseCommand.cpp$(PreprocessSuffix): ./KafkaClient/BaseCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/KafkaClient_BaseCommand.cpp$(PreprocessSuffix) ./KafkaClient/BaseCommand.cpp

$(IntermediateDirectory)/KafkaClient_kafkaTest.cpp$(ObjectSuffix): ./KafkaClient/kafkaTest.cpp $(IntermediateDirectory)/KafkaClient_kafkaTest.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./KafkaClient/kafkaTest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/KafkaClient_kafkaTest.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/KafkaClient_kafkaTest.cpp$(DependSuffix): ./KafkaClient/kafkaTest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/KafkaClient_kafkaTest.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/KafkaClient_kafkaTest.cpp$(DependSuffix) -MM ./KafkaClient/kafkaTest.cpp

$(IntermediateDirectory)/KafkaClient_kafkaTest.cpp$(PreprocessSuffix): ./KafkaClient/kafkaTest.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/KafkaClient_kafkaTest.cpp$(PreprocessSuffix) ./KafkaClient/kafkaTest.cpp

$(IntermediateDirectory)/KafkaClient_KafkaClient.cpp$(ObjectSuffix): ./KafkaClient/KafkaClient.cpp $(IntermediateDirectory)/KafkaClient_KafkaClient.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./KafkaClient/KafkaClient.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/KafkaClient_KafkaClient.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/KafkaClient_KafkaClient.cpp$(DependSuffix): ./KafkaClient/KafkaClient.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/KafkaClient_KafkaClient.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/KafkaClient_KafkaClient.cpp$(DependSuffix) -MM ./KafkaClient/KafkaClient.cpp

$(IntermediateDirectory)/KafkaClient_KafkaClient.cpp$(PreprocessSuffix): ./KafkaClient/KafkaClient.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/KafkaClient_KafkaClient.cpp$(PreprocessSuffix) ./KafkaClient/KafkaClient.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(WorkspacePath)/Linux_Bin/Temp/


