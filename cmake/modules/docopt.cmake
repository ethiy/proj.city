if(NOT TARGET docopt)
    include(ExternalProject)
    ExternalProject_Add(libdocopt
        PREFIX ${CMAKE_BINARY_DIR}/external/docopt
        GIT_REPOSITORY "https://github.com/docopt/docopt.cpp.git"
        GIT_TAG v0.6.2
        CMAKE_ARGS  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                    -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/external/docopt
                    -DUSE_BOOST_REGEX=OFF
        BUILD_COMMAND cmake --build . --target all
        INSTALL_COMMAND cmake --build . --target install
    )

    set(Docopt_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/external/docopt/include/docopt)
    if(UNIX)
        set(Docopt_LIBRARIES "${CMAKE_BINARY_DIR}/external/docopt/lib/libdocopt.a")
    endif(UNIX)
endif()
