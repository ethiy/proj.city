cmake_minimum_required(VERSION 3.4)

project(3DSceneModel C CXX)

# Includes
include(cmake/compilers.cmake)
include(cmake/dependencies.cmake)

# Targets
file(GLOB_RECURSE Mesh_HEADERS "${3DSceneModel_SOURCE_DIR}/src/UrbanObject/Mesh/*.h")
file(GLOB_RECURSE Mesh_SRC "${3DSceneModel_SOURCE_DIR}/src/UrbanObject/Mesh/*.cpp")

file(GLOB_RECURSE Trigl_HEADERS "${3DSceneModel_SOURCE_DIR}/src/UrbanObject/Triangle/*.h")
file(GLOB_RECURSE Trigl_SRC "${3DSceneModel_SOURCE_DIR}/src/UrbanObject/Triangle/*.cpp")

file(GLOB_RECURSE UObj_HEADERS "${3DSceneModel_SOURCE_DIR}/src/UrbanObject/*.h")
file(GLOB_RECURSE UObj_SRC "${3DSceneModel_SOURCE_DIR}/src/UrbanObject/*.cpp")

add_library(UrbanObject ${UObj_HEADERS} ${UObj_SRC} ${Mesh_HEADERS})
target_link_libraries(UrbanObject ${LIBS})

file(GLOB_RECURSE IO_HEADERS "${3DSceneModel_SOURCE_DIR}/src/IO/*.h")
file(GLOB_RECURSE IO_SRC "${3DSceneModel_SOURCE_DIR}/src/IO/*.cpp")

add_library(IO ${IO_HEADERS} ${IO_SRC} ${Mesh_HEADERS})
target_link_libraries(IO ${LIBS})

set(MAIN_SRC ${3DSceneModel_SOURCE_DIR}/src/main.cpp)
add_executable(main ${MAIN_SRC} ${UObj_HEADERS} ${IO_HEADERS} ${Mesh_HEADERS})
target_link_libraries(main UrbanObject IO ${LIBS})
link_directories(main ${LIBS_DIRS})

# Tests
file(GLOB_RECURSE TEST_SRC "${3DSceneModel_SOURCE_DIR}/src/Tests/*.cpp")

add_executable(test_exec ${TEST_SRC})
target_link_libraries(test_exec Catch)
add_test(NAME test COMMAND test_exec --success --force-color)
