##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=LuaTest
ConfigurationName      :=Release
WorkspacePath          :=/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite
ProjectPath            :=/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite/LuaTest
IntermediateDirectory  :=./Debug
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
OutputFile             :=bin/Release/libLuaTest
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="LuaTest.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../../../purenessscopeserver/Common $(IncludeSwitch)../../../../purenessscopeserver/example-Module/LuaTest/Lua $(IncludeSwitch)../../../../Base/thirdparty/lua-5.3.5/src $(IncludeSwitch)../../../../Base/thirdparty/toluapp-master/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)../../../../Base/thirdparty/toluapp-master/build/lib/libtolua++ 
ArLibs                 :=  "../../../../Base/thirdparty/toluapp-master/build/lib/libtolua++.a" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)Debug 

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
Objects0=$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lapi.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lauxlib.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lbitlib.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lcode.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lctype.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldblib.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldo.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_linit.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_liolib.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_llex.c$(ObjectSuffix) \
	$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lmathlib.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lmem.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_loadlib.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lparser.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstrlib.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltable.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltablib.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_luac.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lgc.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lutf8lib.c$(ObjectSuffix) \
	$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lzio.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lvm.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lobject.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lbaselib.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_loslib.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstate.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lcorolib.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_LuaTest.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltm.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldebug.c$(ObjectSuffix) \
	$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldump.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstring.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lundump.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lfunc.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lopcodes.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_Lua_LuaFn.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_BaseCommand.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/data/develop/Lib/Network/PSS-master/Build/purenessscopeserver/Prj_CodeLite/.build-release/LuaTest"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lapi.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lapi.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lapi.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/lapi.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lapi.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lapi.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lapi.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lapi.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lapi.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/lapi.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lapi.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lapi.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lapi.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/lapi.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lauxlib.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lauxlib.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lauxlib.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/lauxlib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lauxlib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lauxlib.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lauxlib.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lauxlib.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lauxlib.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/lauxlib.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lauxlib.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lauxlib.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lauxlib.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/lauxlib.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lbitlib.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lbitlib.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lbitlib.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/lbitlib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lbitlib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lbitlib.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lbitlib.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lbitlib.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lbitlib.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/lbitlib.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lbitlib.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lbitlib.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lbitlib.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/lbitlib.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lcode.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lcode.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lcode.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/lcode.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lcode.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lcode.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lcode.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lcode.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lcode.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/lcode.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lcode.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lcode.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lcode.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/lcode.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lctype.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lctype.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lctype.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/lctype.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lctype.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lctype.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lctype.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lctype.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lctype.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/lctype.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lctype.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lctype.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lctype.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/lctype.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldblib.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/ldblib.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldblib.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/ldblib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldblib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldblib.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/ldblib.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldblib.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldblib.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/ldblib.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldblib.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/ldblib.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldblib.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/ldblib.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldo.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/ldo.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldo.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/ldo.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldo.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldo.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/ldo.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldo.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldo.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/ldo.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldo.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/ldo.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldo.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/ldo.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_linit.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/linit.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_linit.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/linit.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_linit.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_linit.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/linit.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_linit.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_linit.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/linit.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_linit.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/linit.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_linit.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/linit.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_liolib.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/liolib.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_liolib.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/liolib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_liolib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_liolib.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/liolib.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_liolib.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_liolib.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/liolib.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_liolib.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/liolib.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_liolib.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/liolib.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_llex.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/llex.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_llex.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/llex.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_llex.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_llex.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/llex.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_llex.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_llex.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/llex.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_llex.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/llex.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_llex.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/llex.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lmathlib.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lmathlib.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lmathlib.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/lmathlib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lmathlib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lmathlib.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lmathlib.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lmathlib.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lmathlib.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/lmathlib.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lmathlib.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lmathlib.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lmathlib.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/lmathlib.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lmem.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lmem.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lmem.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/lmem.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lmem.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lmem.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lmem.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lmem.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lmem.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/lmem.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lmem.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lmem.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lmem.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/lmem.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_loadlib.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/loadlib.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_loadlib.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/loadlib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_loadlib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_loadlib.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/loadlib.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_loadlib.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_loadlib.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/loadlib.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_loadlib.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/loadlib.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_loadlib.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/loadlib.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lparser.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lparser.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lparser.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/lparser.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lparser.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lparser.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lparser.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lparser.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lparser.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/lparser.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lparser.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lparser.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lparser.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/lparser.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstrlib.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lstrlib.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstrlib.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/lstrlib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstrlib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstrlib.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lstrlib.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstrlib.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstrlib.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/lstrlib.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstrlib.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lstrlib.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstrlib.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/lstrlib.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltable.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/ltable.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltable.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/ltable.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltable.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltable.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/ltable.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltable.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltable.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/ltable.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltable.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/ltable.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltable.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/ltable.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltablib.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/ltablib.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltablib.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/ltablib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltablib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltablib.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/ltablib.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltablib.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltablib.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/ltablib.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltablib.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/ltablib.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltablib.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/ltablib.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_luac.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/luac.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_luac.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/luac.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_luac.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_luac.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/luac.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_luac.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_luac.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/luac.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_luac.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/luac.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_luac.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/luac.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lgc.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lgc.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lgc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/lgc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lgc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lgc.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lgc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lgc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lgc.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/lgc.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lgc.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lgc.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lgc.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/lgc.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lutf8lib.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lutf8lib.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lutf8lib.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/lutf8lib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lutf8lib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lutf8lib.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lutf8lib.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lutf8lib.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lutf8lib.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/lutf8lib.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lutf8lib.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lutf8lib.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lutf8lib.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/lutf8lib.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lzio.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lzio.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lzio.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/lzio.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lzio.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lzio.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lzio.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lzio.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lzio.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/lzio.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lzio.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lzio.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lzio.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/lzio.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lvm.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lvm.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lvm.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/lvm.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lvm.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lvm.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lvm.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lvm.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lvm.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/lvm.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lvm.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lvm.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lvm.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/lvm.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lobject.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lobject.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lobject.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/lobject.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lobject.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lobject.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lobject.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lobject.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lobject.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/lobject.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lobject.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lobject.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lobject.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/lobject.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lbaselib.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lbaselib.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lbaselib.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/lbaselib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lbaselib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lbaselib.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lbaselib.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lbaselib.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lbaselib.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/lbaselib.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lbaselib.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lbaselib.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lbaselib.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/lbaselib.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_loslib.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/loslib.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_loslib.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/loslib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_loslib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_loslib.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/loslib.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_loslib.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_loslib.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/loslib.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_loslib.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/loslib.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_loslib.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/loslib.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstate.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lstate.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstate.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/lstate.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstate.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstate.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lstate.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstate.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstate.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/lstate.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstate.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lstate.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstate.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/lstate.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lcorolib.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lcorolib.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lcorolib.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/lcorolib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lcorolib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lcorolib.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lcorolib.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lcorolib.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lcorolib.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/lcorolib.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lcorolib.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lcorolib.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lcorolib.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/lcorolib.c

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_LuaTest.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/LuaTest/LuaTest.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_LuaTest.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/LuaTest/LuaTest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_LuaTest.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_LuaTest.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/LuaTest/LuaTest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_LuaTest.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_LuaTest.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/LuaTest/LuaTest.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_LuaTest.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/LuaTest/LuaTest.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_LuaTest.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/LuaTest/LuaTest.cpp

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltm.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/ltm.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltm.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/ltm.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltm.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltm.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/ltm.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltm.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltm.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/ltm.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltm.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/ltm.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ltm.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/ltm.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldebug.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/ldebug.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldebug.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/ldebug.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldebug.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldebug.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/ldebug.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldebug.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldebug.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/ldebug.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldebug.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/ldebug.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldebug.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/ldebug.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldump.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/ldump.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldump.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/ldump.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldump.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldump.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/ldump.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldump.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldump.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/ldump.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldump.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/ldump.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_ldump.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/ldump.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstring.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lstring.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstring.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/lstring.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstring.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstring.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lstring.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstring.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstring.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/lstring.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstring.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lstring.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lstring.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/lstring.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lundump.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lundump.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lundump.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/lundump.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lundump.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lundump.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lundump.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lundump.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lundump.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/lundump.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lundump.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lundump.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lundump.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/lundump.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lfunc.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lfunc.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lfunc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/lfunc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lfunc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lfunc.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lfunc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lfunc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lfunc.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/lfunc.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lfunc.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lfunc.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lfunc.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/lfunc.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lopcodes.c$(ObjectSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lopcodes.c $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lopcodes.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/Base/thirdparty/lua-5.3.5/src/lopcodes.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lopcodes.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lopcodes.c$(DependSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lopcodes.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lopcodes.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lopcodes.c$(DependSuffix) -MM ../../../../Base/thirdparty/lua-5.3.5/src/lopcodes.c

$(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lopcodes.c$(PreprocessSuffix): ../../../../Base/thirdparty/lua-5.3.5/src/lopcodes.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_Base_thirdparty_lua-5.3.5_src_lopcodes.c$(PreprocessSuffix) ../../../../Base/thirdparty/lua-5.3.5/src/lopcodes.c

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_Lua_LuaFn.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/LuaTest/Lua/LuaFn.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_Lua_LuaFn.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/LuaTest/Lua/LuaFn.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_Lua_LuaFn.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_Lua_LuaFn.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/LuaTest/Lua/LuaFn.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_Lua_LuaFn.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_Lua_LuaFn.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/LuaTest/Lua/LuaFn.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_Lua_LuaFn.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/LuaTest/Lua/LuaFn.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_Lua_LuaFn.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/LuaTest/Lua/LuaFn.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_BaseCommand.cpp$(ObjectSuffix): ../../../../purenessscopeserver/example-Module/LuaTest/BaseCommand.cpp $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_BaseCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/data/develop/Lib/Network/PSS-master/purenessscopeserver/example-Module/LuaTest/BaseCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_BaseCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_BaseCommand.cpp$(DependSuffix): ../../../../purenessscopeserver/example-Module/LuaTest/BaseCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_BaseCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_BaseCommand.cpp$(DependSuffix) -MM ../../../../purenessscopeserver/example-Module/LuaTest/BaseCommand.cpp

$(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_BaseCommand.cpp$(PreprocessSuffix): ../../../../purenessscopeserver/example-Module/LuaTest/BaseCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_purenessscopeserver_example-Module_LuaTest_BaseCommand.cpp$(PreprocessSuffix) ../../../../purenessscopeserver/example-Module/LuaTest/BaseCommand.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


