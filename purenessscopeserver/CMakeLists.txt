file(GLOB PSS_SRC *.h *.hpp *.cpp *.cc *.c
    FrameCore/*.h FrameCore/*.hpp FrameCore/*.cpp FrameCore/*.cc FrameCore/*.c
    FrameCore/Common/*.h FrameCore/Common/*.hpp FrameCore/Common/*.cpp FrameCore/Common/*.cc FrameCore/Common/*.c
    #some other files
    )
    
if(UNIX)
    #Set rpath
    #set(CMAKE_INSTALL_RPATH "./lib/" "../lib/")
    #set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
endif()

add_executable(pss ${PSS_SRC})

if(UNIX)
    target_link_libraries(pss pthread dl)
else(UNIX)
    target_link_libraries(pss)
endif(UNIX)

set_target_properties(pss PROPERTIES OUTPUT_NAME_DEBUG "app_d")
set_target_properties(pss PROPERTIES
    FOLDER "pss"
    ARCHIVE_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR})
