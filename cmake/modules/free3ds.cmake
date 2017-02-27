if(NOT TARGET free3ds)
    include(ExternalProject)
    ExternalProject_Add(libfree3ds
        PREFIX ${CMAKE_BINARY_DIR}/external/free3ds
        GIT_REPOSITORY "https://github.com/Ethiy/free3ds.git"
        GIT_TAG 24cc51d8677b779d84600d6ffa1b7afc1058a4cf
        CMAKE_ARGS  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                    -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/external/free3ds
                    -DCMAKE_C_FLAGS= "-O3"
                    -DCMAKE_CXX_FLAGS="-O3"
        BUILD_COMMAND cmake --build . --target all
        INSTALL_COMMAND cmake --build . --target install
    )

    set(LIB3DS_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/external/free3ds/include)
    if(UNIX)
        if(APPLE)
            set(LIB3DS_LIBRARIES "${CMAKE_BINARY_DIR}/external/free3ds/lib/libfree3ds.dylib")
        else(APPLE)
            set(LIB3DS_LIBRARIES "${CMAKE_BINARY_DIR}/external/free3ds/lib/libfree3ds.so")
        endif(APPLE)
    endif(UNIX)
endif()
