if(UNIX)
	set(ACE_ROOT /root/Tools/ACE_wrappers)
else()
	set(ACE_ROOT C:/Tools/ACE-6.3.0/ACE_wrappers)
endif()

# TODO:这里要区分平台吧
set(BIN_OUTPUT_DIR ${ROOT_DIR}/Build/Linux/)
set(LIB_OUTPUT_DIR ${ROOT_DIR}/Build/Linux/)

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
	${ROOT_DIR}/FrameCore)

#file(GLOB_RECURSE PSS_SRC *.h *.cpp)

#file(GLOB PSS_SRC *.h *.cpp
#Common/*.h Common/*.cpp 
#FrameCore/Common/*.h FrameCore/Common/*.cpp 
#FrameCore/Console/*.h FrameCore/Console/*.cpp 
#FrameCore/LogSystem/*.h FrameCore/LogSystem/*.cpp 
#FrameCore/Message/*.h FrameCore/Message/*.cpp 
#FrameCore/PacketParse/*.h FrameCore/PacketParse/*.cpp 
#FrameCore/Reactor/*.h FrameCore/Reactor/*.cpp 
#FrameCore/UDP/*.h FrameCore/UDP/*.cpp 
#FrameCore/TinyXML/*.h FrameCore/TinyXML/*.cpp 
#FrameCore/Mail/*.h FrameCore/Mail/*.cpp 
#FrameCore/FileTest/*.h FrameCore/FileTest/*.cpp 
#FrameCore/*.h FrameCore/*.cpp)

file(GLOB_RECURSE COMMON_FILES Common/*.h Common/*.cpp FrameCore/Common/*.h FrameCore/Common/*.cpp)
source_group("Common" FILES ${COMMON_FILES})

file(GLOB_RECURSE CONSOLE_FILES FrameCore/Console/*.h FrameCore/Console/*.cpp)
source_group("Console" FILES ${CONSOLE_FILES})

file(GLOB_RECURSE LOG_SYSTEM_FILES FrameCore/LogSystem/*.h FrameCore/LogSystem/*.cpp)
source_group("LogSystem" FILES ${LOG_SYSTEM_FILES})

file(GLOB_RECURSE MESSAGE_FILES FrameCore/Message/*.h FrameCore/Message/*.cpp)
source_group("Message" FILES ${MESSAGE_FILES})

#TODO: add other source files

link_directories(${ACE_ROOT}/lib)

add_executable(pss 
	${COMMON_FILES}
	${CONSOLE_FILES}
	${LOG_SYSTEM_FILES}
	${MESSAGE_FILES})

if(UNIX)
	target_link_libraries(pss ace)	
else()
    target_link_libraries(pss 
    debug aced.lib
    optimized ace.lib)    
endif()

set_target_properties(pss PROPERTIES OUTPUT_NAME_DEBUG "pss_d") #TODO:if you have another name, please change it.
set_target_properties(pss PROPERTIES
    FOLDER "pss"
    ARCHIVE_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR})
