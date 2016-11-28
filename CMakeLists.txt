cmake_minimum_required(VERSION 3.4)

project(3DSceneModel C CXX)

# Includes
include(cmake/compilers.cmake)
include(cmake/dependencies.cmake)

file(GLOB_RECURSE UObjects_HEADERS "${3DSceneModel_SOURCE_DIR}/src/UObjects/*.h")
file(GLOB_RECURSE UObjects_SRC "${3DSceneModel_SOURCE_DIR}/src/UObjects/*.cpp")

add_library(UrbanObject ${UObjects_HEADERS} ${UObjects_SRC})
target_link_libraries(UrbanObject ${LIBS})

set(MAIN_SRC ${3DSceneModel_SOURCE_DIR}/src/main.cpp)
add_executable(main ${MAIN_SRC} ${UObjects_HEADERS})
target_link_libraries(main UrbanObject ${LIBS})
link_directories(main ${LIBS_DIRS})
