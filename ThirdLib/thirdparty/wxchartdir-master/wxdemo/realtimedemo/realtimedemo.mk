##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=realtimedemo
ConfigurationName      :=Release
WorkspacePath          :=/data/develop/Lib/Network/PSS-master/purenessscopeserver/purenessscopeserver/PSS_CB
ProjectPath            :=/data/develop/Lib/Wx/wxchartdir-master/wxdemo/realtimedemo
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=fox mulder
Date                   :=2019年02月23日
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="realtimedemo.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=   $(shell wx-config --debug=no --libs --unicode=yes)
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)/data/develop/Lib/Wx/wxchartdir-master/wxdemo/common/ $(IncludeSwitch)/data/develop/Lib/Wx/wxchartdir-master/include/ 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)chartdir 
ArLibs                 :=  "libchartdir.so" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)/data/develop/Lib/Wx/wxchartdir-master/lib/linux64 

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
Objects0=$(IntermediateDirectory)/up_common_wxchartviewer.cpp$(ObjectSuffix) $(IntermediateDirectory)/realtimedemoapp.cpp$(ObjectSuffix) $(IntermediateDirectory)/realtimedemo.cpp$(ObjectSuffix) 



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
	@test -d ./Release || $(MakeDirCommand) ./Release


$(IntermediateDirectory)/.d:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_common_wxchartviewer.cpp$(ObjectSuffix): ../common/wxchartviewer.cpp $(IntermediateDirectory)/up_common_wxchartviewer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Wx/wxchartdir-master/wxdemo/common/wxchartviewer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_common_wxchartviewer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_common_wxchartviewer.cpp$(DependSuffix): ../common/wxchartviewer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_common_wxchartviewer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_common_wxchartviewer.cpp$(DependSuffix) -MM ../common/wxchartviewer.cpp

$(IntermediateDirectory)/up_common_wxchartviewer.cpp$(PreprocessSuffix): ../common/wxchartviewer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_common_wxchartviewer.cpp$(PreprocessSuffix) ../common/wxchartviewer.cpp

$(IntermediateDirectory)/realtimedemoapp.cpp$(ObjectSuffix): realtimedemoapp.cpp $(IntermediateDirectory)/realtimedemoapp.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Wx/wxchartdir-master/wxdemo/realtimedemo/realtimedemoapp.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/realtimedemoapp.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/realtimedemoapp.cpp$(DependSuffix): realtimedemoapp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/realtimedemoapp.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/realtimedemoapp.cpp$(DependSuffix) -MM realtimedemoapp.cpp

$(IntermediateDirectory)/realtimedemoapp.cpp$(PreprocessSuffix): realtimedemoapp.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/realtimedemoapp.cpp$(PreprocessSuffix) realtimedemoapp.cpp

$(IntermediateDirectory)/realtimedemo.cpp$(ObjectSuffix): realtimedemo.cpp $(IntermediateDirectory)/realtimedemo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Wx/wxchartdir-master/wxdemo/realtimedemo/realtimedemo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/realtimedemo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/realtimedemo.cpp$(DependSuffix): realtimedemo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/realtimedemo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/realtimedemo.cpp$(DependSuffix) -MM realtimedemo.cpp

$(IntermediateDirectory)/realtimedemo.cpp$(PreprocessSuffix): realtimedemo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/realtimedemo.cpp$(PreprocessSuffix) realtimedemo.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


