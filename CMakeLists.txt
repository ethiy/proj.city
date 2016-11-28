cmake_minimum_required(VERSION 3.4)

project(3DSceneModel C CXX)

# Includes
include(cmake/compilers.cmake)
include(cmake/dependencies.cmake)

file(GLOB_RECURSE UObj_HEADERS "${3DSceneModel_SOURCE_DIR}/src/UrbanObject/*.h")
file(GLOB_RECURSE UObj_SRC "${3DSceneModel_SOURCE_DIR}/src/UrbanObject/*.cpp")

add_library(UrbanObject ${UObj_HEADERS} ${UObj_SRC})
target_link_libraries(UrbanObject ${LIBS})

set(MAIN_SRC ${3DSceneModel_SOURCE_DIR}/src/main.cpp)
add_executable(main ${MAIN_SRC} ${UObj_HEADERS})
target_link_libraries(main UrbanObject ${LIBS})
link_directories(main ${LIBS_DIRS})
