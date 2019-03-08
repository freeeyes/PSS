##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=realtimesweep
ConfigurationName      :=Release
WorkspacePath          :=/data/develop/Lib/Network/PSS-master/purenessscopeserver/purenessscopeserver/PSS_CB
ProjectPath            :=/data/develop/Lib/Wx/wxchartdir-master/wxdemo/realtimesweep
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
ObjectsFileList        :="realtimesweep.txt"
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
Objects0=$(IntermediateDirectory)/randomwave.cpp$(ObjectSuffix) $(IntermediateDirectory)/wxchartviewer.cpp$(ObjectSuffix) $(IntermediateDirectory)/realtimesweepapp.cpp$(ObjectSuffix) $(IntermediateDirectory)/realtimesweep.cpp$(ObjectSuffix) 



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
$(IntermediateDirectory)/randomwave.cpp$(ObjectSuffix): randomwave.cpp $(IntermediateDirectory)/randomwave.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Wx/wxchartdir-master/wxdemo/realtimesweep/randomwave.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/randomwave.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/randomwave.cpp$(DependSuffix): randomwave.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/randomwave.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/randomwave.cpp$(DependSuffix) -MM randomwave.cpp

$(IntermediateDirectory)/randomwave.cpp$(PreprocessSuffix): randomwave.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/randomwave.cpp$(PreprocessSuffix) randomwave.cpp

$(IntermediateDirectory)/wxchartviewer.cpp$(ObjectSuffix): wxchartviewer.cpp $(IntermediateDirectory)/wxchartviewer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Wx/wxchartdir-master/wxdemo/realtimesweep/wxchartviewer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wxchartviewer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wxchartviewer.cpp$(DependSuffix): wxchartviewer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/wxchartviewer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/wxchartviewer.cpp$(DependSuffix) -MM wxchartviewer.cpp

$(IntermediateDirectory)/wxchartviewer.cpp$(PreprocessSuffix): wxchartviewer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wxchartviewer.cpp$(PreprocessSuffix) wxchartviewer.cpp

$(IntermediateDirectory)/realtimesweepapp.cpp$(ObjectSuffix): realtimesweepapp.cpp $(IntermediateDirectory)/realtimesweepapp.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Wx/wxchartdir-master/wxdemo/realtimesweep/realtimesweepapp.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/realtimesweepapp.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/realtimesweepapp.cpp$(DependSuffix): realtimesweepapp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/realtimesweepapp.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/realtimesweepapp.cpp$(DependSuffix) -MM realtimesweepapp.cpp

$(IntermediateDirectory)/realtimesweepapp.cpp$(PreprocessSuffix): realtimesweepapp.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/realtimesweepapp.cpp$(PreprocessSuffix) realtimesweepapp.cpp

$(IntermediateDirectory)/realtimesweep.cpp$(ObjectSuffix): realtimesweep.cpp $(IntermediateDirectory)/realtimesweep.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Wx/wxchartdir-master/wxdemo/realtimesweep/realtimesweep.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/realtimesweep.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/realtimesweep.cpp$(DependSuffix): realtimesweep.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/realtimesweep.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/realtimesweep.cpp$(DependSuffix) -MM realtimesweep.cpp

$(IntermediateDirectory)/realtimesweep.cpp$(PreprocessSuffix): realtimesweep.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/realtimesweep.cpp$(PreprocessSuffix) realtimesweep.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


