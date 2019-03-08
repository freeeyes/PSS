##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=wxdemo
ConfigurationName      :=Release
WorkspacePath          :=/data/develop/Lib/Network/PSS-master/purenessscopeserver/purenessscopeserver/PSS_CB
ProjectPath            :=/data/develop/Lib/Wx/wxchartdir-master/wxdemo/wxdemo
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
ObjectsFileList        :="wxdemo.txt"
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
Objects0=$(IntermediateDirectory)/wxdemoapp.cpp$(ObjectSuffix) $(IntermediateDirectory)/wxdemo.cpp$(ObjectSuffix) $(IntermediateDirectory)/wxchartviewer.cpp$(ObjectSuffix) $(IntermediateDirectory)/democharts.cpp$(ObjectSuffix) $(IntermediateDirectory)/hotspotdialog.cpp$(ObjectSuffix) 



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
$(IntermediateDirectory)/wxdemoapp.cpp$(ObjectSuffix): wxdemoapp.cpp $(IntermediateDirectory)/wxdemoapp.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Wx/wxchartdir-master/wxdemo/wxdemo/wxdemoapp.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wxdemoapp.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wxdemoapp.cpp$(DependSuffix): wxdemoapp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/wxdemoapp.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/wxdemoapp.cpp$(DependSuffix) -MM wxdemoapp.cpp

$(IntermediateDirectory)/wxdemoapp.cpp$(PreprocessSuffix): wxdemoapp.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wxdemoapp.cpp$(PreprocessSuffix) wxdemoapp.cpp

$(IntermediateDirectory)/wxdemo.cpp$(ObjectSuffix): wxdemo.cpp $(IntermediateDirectory)/wxdemo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Wx/wxchartdir-master/wxdemo/wxdemo/wxdemo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wxdemo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wxdemo.cpp$(DependSuffix): wxdemo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/wxdemo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/wxdemo.cpp$(DependSuffix) -MM wxdemo.cpp

$(IntermediateDirectory)/wxdemo.cpp$(PreprocessSuffix): wxdemo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wxdemo.cpp$(PreprocessSuffix) wxdemo.cpp

$(IntermediateDirectory)/wxchartviewer.cpp$(ObjectSuffix): wxchartviewer.cpp $(IntermediateDirectory)/wxchartviewer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Wx/wxchartdir-master/wxdemo/wxdemo/wxchartviewer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wxchartviewer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wxchartviewer.cpp$(DependSuffix): wxchartviewer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/wxchartviewer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/wxchartviewer.cpp$(DependSuffix) -MM wxchartviewer.cpp

$(IntermediateDirectory)/wxchartviewer.cpp$(PreprocessSuffix): wxchartviewer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wxchartviewer.cpp$(PreprocessSuffix) wxchartviewer.cpp

$(IntermediateDirectory)/democharts.cpp$(ObjectSuffix): democharts.cpp $(IntermediateDirectory)/democharts.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Wx/wxchartdir-master/wxdemo/wxdemo/democharts.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/democharts.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/democharts.cpp$(DependSuffix): democharts.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/democharts.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/democharts.cpp$(DependSuffix) -MM democharts.cpp

$(IntermediateDirectory)/democharts.cpp$(PreprocessSuffix): democharts.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/democharts.cpp$(PreprocessSuffix) democharts.cpp

$(IntermediateDirectory)/hotspotdialog.cpp$(ObjectSuffix): hotspotdialog.cpp $(IntermediateDirectory)/hotspotdialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Wx/wxchartdir-master/wxdemo/wxdemo/hotspotdialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hotspotdialog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hotspotdialog.cpp$(DependSuffix): hotspotdialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hotspotdialog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/hotspotdialog.cpp$(DependSuffix) -MM hotspotdialog.cpp

$(IntermediateDirectory)/hotspotdialog.cpp$(PreprocessSuffix): hotspotdialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hotspotdialog.cpp$(PreprocessSuffix) hotspotdialog.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


