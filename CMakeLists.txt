cmake_minimum_required(VERSION 3.2)

project("proj.city"
        VERSION 0.4.1
        LANGUAGES CXX)

##
# ________________________________________________ Modules __________________________________________________
##
include(cmake/compilers.cmake)
include(cmake/dependencies.cmake)

##
# _______________________________________________ Libraries __________________________________________________
##

add_subdirectory(src/libs libs)

##
# ___________________________________________________ Samples ___________________________________________________
##

add_subdirectory(src/samples samples)

##
# ___________________________________________________ Samples ___________________________________________________
##

add_subdirectory(src/cli cli)

##
# ____________________________________________________ Tests _____________________________________________________
##

add_subdirectory(src/tests tests)
add_test(NAME test COMMAND tests --reporter compact)
