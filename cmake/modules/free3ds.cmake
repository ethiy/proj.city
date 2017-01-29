if(NOT TARGET free3ds)
    include(ExternalProject)
    ExternalProject_Add(free3ds
        PREFIX ${CMAKE_BINARY_DIR}/external/free3ds
        GIT_REPOSITORY "https://github.com/Ethiy/free3ds.git"
        GIT_TAG 834c6579c6768a73dca41b7c14e9daba4491ebce
        UPDATE_COMMAND ""
        INSTALL_DIR ${CMAKE_BINARY_DIR}/external/free3ds
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                    -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/external/free3ds
                    -DBUILD_STATIC_LIBS=ON
                    -DBUILD_PACKAGING=OFF
                    -DBUILD_TESTING=OFF
                    -DBUILD_NC_TESTS=OFF
                    -BUILD_CONFIG_TESTS=OFF
                    -DINSTALL_HEADERS=ON
                    -DCMAKE_C_FLAGS=${CMAKE_CXX_FLAGS}
                    -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
        LOG_DOWNLOAD 1
        LOG_INSTALL 1
    )

    set(LIB3DS_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/external/free3ds/include)
    set(LIB3DS_LIBRARIES ${CMAKE_BINARY_DIR}/external/free3ds/lib/libfree3ds.a )
endif()
