##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=Test
ConfigurationName      :=Release
WorkspacePath          :=../Build/Linux
ProjectPath            :=../CppUnitTest
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
Preprocessors          :=$(PreprocessorSwitch)_LINUX_PLATFORM_ 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="Test.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -O0
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../ThirdLib/thirdparty $(IncludeSwitch)../ThirdLib/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)pthread 
ArLibs                 :=  "libpthread.so" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)Release 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -Wall $(Preprocessors)
CFLAGS   :=   $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/Test_testcase.cpp$(ObjectSuffix) $(IntermediateDirectory)/Test_test_linux.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_ThirdLib_thirdparty_sock_wrap.cpp$(ObjectSuffix) 



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
$(IntermediateDirectory)/Test_testcase.cpp$(ObjectSuffix): ./Test/testcase.cpp $(IntermediateDirectory)/Test_testcase.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./Test/testcase.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Test_testcase.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Test_testcase.cpp$(DependSuffix): ./Test/testcase.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Test_testcase.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Test_testcase.cpp$(DependSuffix) -MM ./Test/testcase.cpp

$(IntermediateDirectory)/Test_testcase.cpp$(PreprocessSuffix): ./Test/testcase.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Test_testcase.cpp$(PreprocessSuffix) ./Test/testcase.cpp

$(IntermediateDirectory)/Test_test_linux.cpp$(ObjectSuffix): ./Test/test_linux.cpp $(IntermediateDirectory)/Test_test_linux.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./Test/test_linux.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Test_test_linux.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Test_test_linux.cpp$(DependSuffix): ./Test/test_linux.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Test_test_linux.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Test_test_linux.cpp$(DependSuffix) -MM ./Test/test_linux.cpp

$(IntermediateDirectory)/Test_test_linux.cpp$(PreprocessSuffix): ./Test/test_linux.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Test_test_linux.cpp$(PreprocessSuffix) ./Test/test_linux.cpp

$(IntermediateDirectory)/up_ThirdLib_thirdparty_sock_wrap.cpp$(ObjectSuffix): ../ThirdLib/thirdparty/sock_wrap.cpp $(IntermediateDirectory)/up_ThirdLib_thirdparty_sock_wrap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../ThirdLib/thirdparty/sock_wrap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_ThirdLib_thirdparty_sock_wrap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_ThirdLib_thirdparty_sock_wrap.cpp$(DependSuffix): ../ThirdLib/thirdparty/sock_wrap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_ThirdLib_thirdparty_sock_wrap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_ThirdLib_thirdparty_sock_wrap.cpp$(DependSuffix) -MM ../ThirdLib/thirdparty/sock_wrap.cpp

$(IntermediateDirectory)/up_ThirdLib_thirdparty_sock_wrap.cpp$(PreprocessSuffix): ../ThirdLib/thirdparty/sock_wrap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_ThirdLib_thirdparty_sock_wrap.cpp$(PreprocessSuffix) ../ThirdLib/thirdparty/sock_wrap.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(WorkspacePath)/Temp/


