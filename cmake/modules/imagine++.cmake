if(NOT TARGET imagine++)
    include(ExternalProject)
    ExternalProject_Add(libimagine++
        PREFIX ${CMAKE_BINARY_DIR}/external/imagine++
        GIT_REPOSITORY "https://github.com/Ethiy/imagine-pp.git"
        GIT_TAG 7c8899bca0d3fa0559e22dd7dce799cdce1f4efd
        CMAKE_ARGS  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                    -DCMAKE_INSTALL_PREFIX=/usr/local
                    -DCMAKE_C_FLAGS= "-O3"
                    -DCMAKE_CXX_FLAGS="-O3"
        BUILD_COMMAND cmake --build . --target all
        INSTALL_COMMAND cmake --build . --target install
    )
endif()
