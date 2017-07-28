cmake_minimum_required(VERSION 3.2)

project("proj.city"
        VERSION 0.4.1
        LANGUAGES CXX
)

configure_file (
        "${PROJECT_SOURCE_DIR}/src/config.h.in"
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

include_directories("${PROJECT_SOURCE_DIR}/src/libs")
add_subdirectory(src/libs libs)

##
# ___________________________________________________ Samples ___________________________________________________
##

add_subdirectory(src/bin bin)

##
# ____________________________________________________ Tests _____________________________________________________
##

add_subdirectory(src/tests tests)
add_test(NAME test COMMAND tests --reporter compact)
