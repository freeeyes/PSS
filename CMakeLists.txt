cmake_minimum_required(VERSION 3.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(pss)

#check ace environmental variable
if(NOT DEFINED ENV{ACE_ROOT})
	message("ACE no find install VS_DEVENV_PATH=${VS_DEVENV_PATH}")
else()
	message("ACE find install VS_DEVENV_PATH=${VS_DEVENV_PATH}")
	
	#check cxx flag in linux system
	if(UNIX)
		#linux
		include(CheckCXXCompilerFlag)
		CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
		if(COMPILER_SUPPORTS_CXX11)
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
		endif()
	endif()

	########################################################################

	set(ROOT_DIR ${PROJECT_SOURCE_DIR})

	if(UNIX)
		#linux
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -ggdb -fPIC")
		if(CMAKE_BUILD_TYPE MATCHES "debug")
			set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fPIC")
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -D_DEBUG ")
		else()
			set(CMAKE_BUILD_TYPE "Release")
			set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fPIC")
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -DNDEBUG")
		endif()
		set(LDFLAGS "$ENV{LDFLAGS} -stdlib=libc++")
		#TODO:3rd lib dir
		set(DEP_LIB_DIR ${ACE_ROOT}/lib/)
		add_definitions(-Wno-unused-variable -Wno-unused-parameter -Wno-unused-function -Wno-strict-aliasing -Wno-pmf-conversions)
		
		#OPTION:coverage
		#if(ENABLE_COVERAGE)
		#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
		#endif()
		
	else()
		#windows
		set(DEP_LIB_DIR ${ACE_ROOT}/lib/)
		set(BIN_OUTPUT_DIR ${ROOT_DIR}/Build/Windows)
		add_definitions(
			-DWIN
			-D_WINDOWS
			-D_CRT_SECURE_NO_WARNINGS)
		if(CMAKE_BUILD_TYPE MATCHES "debug")
			add_definitions(-D_DEBUG)
		else(CMAKE_BUILD_TYPE MATCHES "debug")
			add_definitions(-DNDEBUG)
		endif(CMAKE_BUILD_TYPE MATCHES "debug")
	endif()

	#log
	message("ROOT dir=${ROOT_DIR}")
	#message("Bin Output dir=${BIN_OUTPUT_DIR}")
	#message("DEP lib dir=${DEP_LIB_DIR}")
	#message("Plugin Output=${LIB_OUTPUT_DIR}")
	message("CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")

	#codecov
	#find_package(codecov)
	##################################################################
	message("Start to build all...")

	#add your own sub dirs
	add_subdirectory(purenessscopeserver)
	#TODO:add other dirs	
endif()


