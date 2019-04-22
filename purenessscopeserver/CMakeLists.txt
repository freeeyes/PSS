if(UNIX)
set(ACE_ROOT C:\Tools\ACE-6.3.0\ACE_wrappers)
else()
set(ACE_ROOT /root/Tools/ACE_wrappers)
endif()

if(UNIX)
include_directories(
    #TODO:include dir
	${ACE_ROOT}/
	${ROOT_DIR}/Common
	${ROOT_DIR}/FrameCore/Common
	${ROOT_DIR}/FrameCore/Console
	${ROOT_DIR}/FrameCore/LogSystem
	${ROOT_DIR}/FrameCore/Message
	${ROOT_DIR}/FrameCore/PacketParse
	${ROOT_DIR}/FrameCore/Reactor
	${ROOT_DIR}/FrameCore/UDP
	${ROOT_DIR}/FrameCore/TinyXML
	${ROOT_DIR}/FrameCore/Mail
	${ROOT_DIR}/FrameCore/FileTest
	${ROOT_DIR}/FrameCore
	)
	
file(GLOB PSS_SRC *.h *.cpp 
    *.cpp
    Common/*.h Common/*.cpp 
    FrameCore/Common/*.h FrameCore/Common/*.cpp 
	FrameCore/Console/*.h FrameCore/Console/*.cpp 
	FrameCore/LogSystem/*.h FrameCore/LogSystem/*.cpp 
	FrameCore/Message/*.h FrameCore/Message/*.cpp 
	FrameCore/PacketParse/*.h FrameCore/PacketParse/*.cpp 
	FrameCore/Reactor/*.h FrameCore/Reactor/*.cpp 
	FrameCore/UDP/*.h FrameCore/UDP/*.cpp 
	FrameCore/TinyXML/*.h FrameCore/TinyXML/*.cpp 
	FrameCore/Mail/*.h FrameCore/Mail/*.cpp 
	FrameCore/FileTest/*.h FrameCore/FileTest/*.cpp 
	FrameCore/*.h FrameCore/*.cpp
	)	
else()
include_directories(
    #TODO:include dir
	${ACE_ROOT}/
	${ROOT_DIR}/Common
	${ROOT_DIR}/FrameCore/Common
	${ROOT_DIR}/FrameCore/Console
	${ROOT_DIR}/FrameCore/LogSystem
	${ROOT_DIR}/FrameCore/Message
	${ROOT_DIR}/FrameCore/PacketParse
	${ROOT_DIR}/FrameCore/Proactor
	${ROOT_DIR}/FrameCore/UDP
	${ROOT_DIR}/FrameCore/TinyXML
	${ROOT_DIR}/FrameCore/Mail
	${ROOT_DIR}/FrameCore/FileTest
	${ROOT_DIR}/FrameCore
	)

file(GLOB PSS_SRC *.h *.cpp 
	*.cpp
	Common/*.h Common/*.cpp 
    FrameCore/Common/*.h FrameCore/Common/*.cpp 
	FrameCore/Console/*.h FrameCore/Console/*.cpp 
	FrameCore/LogSystem/*.h FrameCore/LogSystem/*.cpp 
	FrameCore/Message/*.h FrameCore/Message/*.cpp 
	FrameCore/PacketParse/*.h FrameCore/PacketParse/*.cpp 
	FrameCore/Reactor/*.h FrameCore/Proactor/*.cpp 
	FrameCore/UDP/*.h FrameCore/UDP/*.cpp 
	FrameCore/TinyXML/*.h FrameCore/TinyXML/*.cpp 
	FrameCore/Mail/*.h FrameCore/Mail/*.cpp 
	FrameCore/FileTest/*.h FrameCore/FileTest/*.cpp 
	FrameCore/*.h FrameCore/*.cpp
	)
endif()


add_executable(pss ${PSS_SRC})

if(UNIX)
	set(DEP_LIB_DIR ${ACE_ROOT}/lib)
	target_link_libraries(libace.so)
    set(BIN_OUTPUT_DIR ${ROOT_DIR}/Build/Linux/)
    set(LIB_OUTPUT_DIR ${ROOT_DIR}/Build/Linux/)	
else()
	set(DEP_LIB_DIR ${ACE_ROOT}/lib)
	set(BIN_OUTPUT_DIR ${ROOT_DIR}/Build/Windows/)
    set(LIB_OUTPUT_DIR ${ROOT_DIR}/Build/Windows/)
    target_link_libraries(
    debug aced.lib
    optimized ace.lib
	)    
endif()

set_target_properties(pss PROPERTIES OUTPUT_NAME_DEBUG "pss_d") #TODO:if you have another name, please change it.
set_target_properties(pss PROPERTIES
    FOLDER "pss"
    ARCHIVE_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR})
