if(NOT TARGET Easyloggingpp)
    include(ExternalProject)
    set(FETCH_EXTERNAL_EASYLOGGING
        GIT_REPOSITORY https://github.com/easylogging/easyloggingpp.git
        GIT_TAG fd4a183e11105ede52a308de61f1c0096a064128)
    ExternalProject_Add(EasyLogging++
        PREFIX ${CMAKE_BINARY_DIR}/external/EasyLogging
        ${FETCH_EXTERNAL_EASYLOGGING}
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/external/EasyLogging/src/EasyLogging++/src/easylogging++.h
                                                 ${CMAKE_BINARY_DIR}/external/EasyLogging/include/easylogging++.h
    )
    add_library(EasyLogging INTERFACE)
    add_dependencies(EasyLogging EasyLogging++)
    target_include_directories(EasyLogging INTERFACE ${CMAKE_BINARY_DIR}/external/EasyLogging/include)
endif()
