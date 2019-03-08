##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=realtimezoomscroll
ConfigurationName      :=Release
WorkspacePath          :=/data/develop/Lib/Network/PSS-master/purenessscopeserver/purenessscopeserver/PSS_CB
ProjectPath            :=/data/develop/Lib/Wx/wxchartdir-master/wxdemo/realtimezoomscroll
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
ObjectsFileList        :="realtimezoomscroll.txt"
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
Objects0=$(IntermediateDirectory)/realtimezoomscrollapp.cpp$(ObjectSuffix) $(IntermediateDirectory)/wxchartviewer.cpp$(ObjectSuffix) $(IntermediateDirectory)/realtimezoomscroll.cpp$(ObjectSuffix) 



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
$(IntermediateDirectory)/realtimezoomscrollapp.cpp$(ObjectSuffix): realtimezoomscrollapp.cpp $(IntermediateDirectory)/realtimezoomscrollapp.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Wx/wxchartdir-master/wxdemo/realtimezoomscroll/realtimezoomscrollapp.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/realtimezoomscrollapp.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/realtimezoomscrollapp.cpp$(DependSuffix): realtimezoomscrollapp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/realtimezoomscrollapp.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/realtimezoomscrollapp.cpp$(DependSuffix) -MM realtimezoomscrollapp.cpp

$(IntermediateDirectory)/realtimezoomscrollapp.cpp$(PreprocessSuffix): realtimezoomscrollapp.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/realtimezoomscrollapp.cpp$(PreprocessSuffix) realtimezoomscrollapp.cpp

$(IntermediateDirectory)/wxchartviewer.cpp$(ObjectSuffix): wxchartviewer.cpp $(IntermediateDirectory)/wxchartviewer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Wx/wxchartdir-master/wxdemo/realtimezoomscroll/wxchartviewer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wxchartviewer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wxchartviewer.cpp$(DependSuffix): wxchartviewer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/wxchartviewer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/wxchartviewer.cpp$(DependSuffix) -MM wxchartviewer.cpp

$(IntermediateDirectory)/wxchartviewer.cpp$(PreprocessSuffix): wxchartviewer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wxchartviewer.cpp$(PreprocessSuffix) wxchartviewer.cpp

$(IntermediateDirectory)/realtimezoomscroll.cpp$(ObjectSuffix): realtimezoomscroll.cpp $(IntermediateDirectory)/realtimezoomscroll.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Wx/wxchartdir-master/wxdemo/realtimezoomscroll/realtimezoomscroll.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/realtimezoomscroll.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/realtimezoomscroll.cpp$(DependSuffix): realtimezoomscroll.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/realtimezoomscroll.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/realtimezoomscroll.cpp$(DependSuffix) -MM realtimezoomscroll.cpp

$(IntermediateDirectory)/realtimezoomscroll.cpp$(PreprocessSuffix): realtimezoomscroll.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/realtimezoomscroll.cpp$(PreprocessSuffix) realtimezoomscroll.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


