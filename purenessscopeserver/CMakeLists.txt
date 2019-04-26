if(UNIX)
	set(ACE_ROOT /root/Tools/ACE_wrappers)
	set(BIN_OUTPUT_DIR ${ROOT_DIR}/Build/Linux/)
	set(LIB_OUTPUT_DIR ${ROOT_DIR}/Build/Linux/)
else()
	set(ACE_ROOT C:/Tools/ACE-6.3.0/ACE_wrappers)
	set(BIN_OUTPUT_DIR ${ROOT_DIR}/Build/Windows/)
	set(LIB_OUTPUT_DIR ${ROOT_DIR}/Build/Windows/)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BIN_OUTPUT_DIR}")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BIN_OUTPUT_DIR}")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${BIN_OUTPUT_DIR}")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${BIN_OUTPUT_DIR}")
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${BIN_OUTPUT_DIR}")
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${BIN_OUTPUT_DIR}")
endif()

# TODO:这里要区分平台吧
if(UNIX)
	include_directories(
		#TODO:include dir
		${ACE_ROOT}/
		./Common
		./FrameCore/Common
		./FrameCore/Console
		./FrameCore/LogSystem
		./FrameCore/Message
		./FrameCore/PacketParse
		./FrameCore/Reactor
		./FrameCore/UDP
		./FrameCore/TinyXML
		./FrameCore/Mail
		./FrameCore/FileTest
		./FrameCore)

	file(GLOB_RECURSE COMMON_FILES Common/*.h Common/*.cpp FrameCore/Common/*.h FrameCore/Common/*.cpp)
	source_group("Common" FILES ${COMMON_FILES})

	file(GLOB_RECURSE CONSOLE_FILES FrameCore/Console/*.h FrameCore/Console/*.cpp)
	source_group("Console" FILES ${CONSOLE_FILES})

	file(GLOB_RECURSE FILETEST_FILES FrameCore/FileTest/*.h FrameCore/FileTest/*.cpp)
	source_group("FileTest" FILES ${FILETEST_FILES})

	file(GLOB_RECURSE LOG_SYSTEM_FILES FrameCore/LogSystem/*.h FrameCore/LogSystem/*.cpp)
	source_group("LogSystem" FILES ${LOG_SYSTEM_FILES})

	file(GLOB_RECURSE MAIL_FILES FrameCore/Mail/*.h FrameCore/Mail/*.cpp)
	source_group("Mail" FILES ${MAIL_FILES})

	file(GLOB_RECURSE MESSAGE_FILES FrameCore/Message/*.h FrameCore/Message/*.cpp)
	source_group("Message" FILES ${MESSAGE_FILES})

	file(GLOB_RECURSE PACKETPARSE_FILES FrameCore/PacketParse/*.h FrameCore/PacketParse/*.cpp)
	source_group("PacketParse" FILES ${PACKETPARSE_FILES})

	file(GLOB_RECURSE REACTOR_FILES FrameCore/Reactor/*.h FrameCore/Reactor/*.cpp)
	source_group("Reactor" FILES ${PROACTOR_FILES})

	file(GLOB_RECURSE XML_FILES FrameCore/TinyXML/*.h FrameCore/TinyXML/*.cpp)
	source_group("TinyXML" FILES ${XML_FILES})

	file(GLOB_RECURSE UDP_FILES FrameCore/UDP/*.h FrameCore/UDP/*.cpp)
	source_group("UDP" FILES ${UDP_FILES})

	file(GLOB_RECURSE MAIN_FILES FrameCore/PurenessScopeServer.cpp)
	source_group("Main" FILES ${MAIN_FILES})

	link_directories(${ACE_ROOT}/lib)

	add_executable(pss 
		${COMMON_FILES}
		${CONSOLE_FILES}
		${FILETEST_FILES}
		${LOG_SYSTEM_FILES}
		${MAIL_FILES}
		${MESSAGE_FILES}
		${PACKETPARSE_FILES}
		${PROACTOR_FILES}
		${XML_FILES}
		${UDP_FILES}
		${MAIN_FILES}
		)
else()
	include_directories(
		#TODO:include dir
		${ACE_ROOT}/
		./Common
		./FrameCore/Common
		./FrameCore/Console
		./FrameCore/LogSystem
		./FrameCore/Message
		./FrameCore/PacketParse
		./FrameCore/Proactor
		./FrameCore/UDP
		./FrameCore/TinyXML
		./FrameCore/Mail
		./FrameCore/FileTest
		./FrameCore)

	file(GLOB_RECURSE COMMON_FILES Common/*.h Common/*.cpp FrameCore/Common/*.h FrameCore/Common/*.cpp)
	source_group("Common" FILES ${COMMON_FILES})

	file(GLOB_RECURSE CONSOLE_FILES FrameCore/Console/*.h FrameCore/Console/*.cpp)
	source_group("Console" FILES ${CONSOLE_FILES})

	file(GLOB_RECURSE FILETEST_FILES FrameCore/FileTest/*.h FrameCore/FileTest/*.cpp)
	source_group("FileTest" FILES ${FILETEST_FILES})

	file(GLOB_RECURSE LOG_SYSTEM_FILES FrameCore/LogSystem/*.h FrameCore/LogSystem/*.cpp)
	source_group("LogSystem" FILES ${LOG_SYSTEM_FILES})

	file(GLOB_RECURSE MAIL_FILES FrameCore/Mail/*.h FrameCore/Mail/*.cpp)
	source_group("Mail" FILES ${MAIL_FILES})

	file(GLOB_RECURSE MESSAGE_FILES FrameCore/Message/*.h FrameCore/Message/*.cpp)
	source_group("Message" FILES ${MESSAGE_FILES})

	file(GLOB_RECURSE PACKETPARSE_FILES FrameCore/PacketParse/*.h FrameCore/PacketParse/*.cpp)
	source_group("PacketParse" FILES ${PACKETPARSE_FILES})

	file(GLOB_RECURSE PROACTOR_FILES FrameCore/Proactor/*.h FrameCore/Proactor/*.cpp)
	source_group("Proactor" FILES ${PROACTOR_FILES})

	file(GLOB_RECURSE XML_FILES FrameCore/TinyXML/*.h FrameCore/TinyXML/*.cpp)
	source_group("TinyXML" FILES ${XML_FILES})

	file(GLOB_RECURSE UDP_FILES FrameCore/UDP/*.h FrameCore/UDP/*.cpp)
	source_group("UDP" FILES ${UDP_FILES})

	file(GLOB_RECURSE MAIN_FILES FrameCore/PurenessScopeServer.cpp)
	source_group("Main" FILES ${MAIN_FILES})

	#TODO: add other source files

	link_directories(${ACE_ROOT}/lib)

	add_executable(pss 
		${COMMON_FILES}
		${CONSOLE_FILES}
		${FILETEST_FILES}
		${LOG_SYSTEM_FILES}
		${MAIL_FILES}
		${MESSAGE_FILES}
		${PACKETPARSE_FILES}
		${PROACTOR_FILES}
		${XML_FILES}
		${UDP_FILES}
		${MAIN_FILES}
		)
endif()

if(UNIX)
	target_link_libraries(pss ace)	
else()
    target_link_libraries(pss 
    debug aced.lib DbgHelp.lib psapi.lib
    optimized ace.lib DbgHelp.lib psapi.lib)    
endif()

set_target_properties(pss PROPERTIES 
	OUTPUT_NAME_DEBUG "pss_d"
	ARCHIVE_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR}) #TODO:if you have another name, please change it.
set_target_properties(pss PROPERTIES
    FOLDER "pss"
    ARCHIVE_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR})
