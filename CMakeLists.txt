cmake_minimum_required(VERSION 3.4)

project(3DSceneModel C CXX)

# Includes
include(cmake/dependencies.cmake)
include(cmake/compilers.cmake)

file(GLOB_RECURSE UObjects_HEADERS "${3DSceneModel_SOURCE_DIR}/src/UObjects/*.h")
file(GLOB_RECURSE UObjects_SRC "${3DSceneModel_SOURCE_DIR}/src/UObjects/*.cpp")

add_library(UrbanObject ${UObjects_HEADERS} ${UObjects_SRC})
target_link_libraries(UrbanObject ${LINKER_LIBS})
