if(NOT TARGET Catch)
    include(ExternalProject)
    if(IS_TRAVIS_BUILD)     # on travis, use git for fetching instead of wget
        set(GB_FETCH_EXTERNAL_CATCH
            GIT_REPOSITORY https://github.com/philsquared/Catch.git
            GIT_TAG 0c1c9fa9229c7358ce16c9910004a39290b20f5f)
    elseif(WIN32)
        set(GB_FETCH_EXTERNAL_CATCH
            URL https://github.com/philsquared/Catch/archive/v1.2.1-develop.12.zip
        )
    else()
        set(GB_FETCH_EXTERNAL_CATCH
            URL https://github.com/philsquared/Catch/archive/v1.2.1-develop.12.tar.gz
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
