if(NOT TARGET Catch)
    include(ExternalProject)
    if(IS_TRAVIS_BUILD)     # on travis, use git for fetching instead of wget
        set(GB_FETCH_EXTERNAL_CATCH
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG 19ab2117c5bac2f376f8da4a4b25e183137bcec0)
    elseif(WIN32)
        set(GB_FETCH_EXTERNAL_CATCH
            URL https://github.com/catchorg/Catch2/archive/v2.0.1.zip
        )
    else()
        set(GB_FETCH_EXTERNAL_CATCH
            URL https://github.com/catchorg/Catch2/archive/v2.0.1.tar.gz
        )
    endif()
    ExternalProject_Add(libcatch
        PREFIX ${CMAKE_BINARY_DIR}/external/Catch
        ${GB_FETCH_EXTERNAL_CATCH}
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/external/Catch/src/libcatch/single_include/catch.hpp
                                                 ${CMAKE_BINARY_DIR}/external/Catch/include/catch.hpp
    )
    add_library(Catch INTERFACE)
    add_dependencies(Catch libcatch)
    target_include_directories(Catch INTERFACE ${CMAKE_BINARY_DIR}/external/Catch/include)
endif()
