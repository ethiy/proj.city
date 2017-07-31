cmake_minimum_required(VERSION 3.2)

project("proj.city"
        VERSION 0.4.1
        LANGUAGES CXX
)

configure_file (
        "${PROJECT_SOURCE_DIR}/src/include/config.h.in"
        "${PROJECT_BINARY_DIR}/config.h"
)

##
# ________________________________________________ Modules __________________________________________________
##
include(cmake/compilers.cmake)
include(cmake/dependencies.cmake)

##
# _______________________________________________ Libraries __________________________________________________
##

include_directories(BEFORE "${PROJECT_SOURCE_DIR}/src/include")
add_subdirectory(src/lib lib)

##
# ___________________________________________________ Samples ___________________________________________________
##

add_subdirectory(src/bin bin)

##
# ____________________________________________________ Tests _____________________________________________________
##

add_subdirectory(src/tests tests)
add_test(NAME test COMMAND tests --reporter compact)
