cmake_minimum_required(VERSION 3.2)

project(3DSceneModel C CXX)

##
# ________________________________________________ Modules __________________________________________________
##
include(cmake/compilers.cmake)
include(cmake/dependencies.cmake)

##
# _______________________________________________ Libraries __________________________________________________
##

# ShadowMesh
file(GLOB_RECURSE ShadowMesh_HEADERS "${3DSceneModel_SOURCE_DIR}/src/ShadowMesh/*.h")
file(GLOB_RECURSE ShadowMesh_SRC "${3DSceneModel_SOURCE_DIR}/src/ShadowMesh/*.cpp")

file(GLOB_RECURSE Face_HEADERS "${3DSceneModel_SOURCE_DIR}/src/ShadowMesh/Face/*.h")
file(GLOB_RECURSE Face_SRC "${3DSceneModel_SOURCE_DIR}/src/ShadowMesh/Face/*.cpp")

add_library(ShadowMesh ${ShadowMesh_HEADERS} ${ShadowMesh_SRC} ${Face_HEADERS})
target_link_libraries(ShadowMesh ${LIBS})

# UrbanObject
file(GLOB_RECURSE UObj_HEADERS "${3DSceneModel_SOURCE_DIR}/src/UrbanObject/*.h")
file(GLOB_RECURSE UObj_SRC "${3DSceneModel_SOURCE_DIR}/src/UrbanObject/*.cpp")

add_library(UrbanObject ${UObj_HEADERS} ${UObj_SRC} ${ShadowMesh_HEADERS})
target_link_libraries(UrbanObject ShadowMesh ${LIBS})

# Porjections
file(GLOB_RECURSE Proj_HEADERS "${3DSceneModel_SOURCE_DIR}/src/Projection/*.h")
file(GLOB_RECURSE Proj_SRC "${3DSceneModel_SOURCE_DIR}/src/Projection/*.cpp")

add_library(Projection ${Proj_HEADERS} ${Proj_SRC})

# Algorithms
file(GLOB_RECURSE Algorithms_HEADERS "${3DSceneModel_SOURCE_DIR}/src/Algorithms/*.h")
file(GLOB_RECURSE Algorithms_SRC "${3DSceneModel_SOURCE_DIR}/src/Algorithms/*.cpp")

add_library(Algorithms ${Algorithms_HEADERS} ${Algorithms_SRC} ${UObj_HEADERS} ${ShadowMesh_HEADERS})
target_link_libraries(Algorithms UrbanObject ShadowMesh ${LIBS})

# Input/Output
file(GLOB_RECURSE Line_HEADERS "${3DSceneModel_SOURCE_DIR}/src/IO/Line/*.h")
file(GLOB_RECURSE Line_SRC "${3DSceneModel_SOURCE_DIR}/src/IO/Line/*.cpp")

add_library(Line ${Line_HEADERS} ${Line_SRC})
target_link_libraries(Line ${LIBS})

file(GLOB_RECURSE IO_HEADERS "${3DSceneModel_SOURCE_DIR}/src/IO/*.h")
file(GLOB_RECURSE IO_SRC "${3DSceneModel_SOURCE_DIR}/src/IO/*.cpp")

add_library(IO ${IO_HEADERS} ${IO_SRC} ${ShadowMesh_HEADERS} ${UObj_HEADERS} ${Line_HEADERS})
target_link_libraries(IO ShadowMesh Line UrbanObject ${LIBS})

##
# ___________________________________________________ Samples ___________________________________________________
##

# Santa
set(SANTA_SRC "${3DSceneModel_SOURCE_DIR}/src/Samples/hello_santa.cpp")
add_executable(hello_santa ${SANTA_SRC} ${UObj_HEADERS} ${IO_HEADERS})
target_link_libraries(hello_santa ShadowMesh UrbanObject IO ${LIBS})
link_directories(hello_santa ${LIBS_DIRS})

# Project Santa
set(Proj_XY_SANTA_SRC "${3DSceneModel_SOURCE_DIR}/src/Samples/project_santa_xy.cpp")
add_executable(project_santa_xy ${Proj_HEADERS} ${Proj_XY_SANTA_SRC} ${UObj_HEADERS} ${IO_HEADERS} ${Algorithms_HEADERS})
target_link_libraries(project_santa_xy Projection Algorithms ShadowMesh UrbanObject IO ${LIBS})
link_directories(project_santa_xy ${LIBS_DIRS})

##
# ____________________________________________________ Tests _____________________________________________________
##

file(GLOB_RECURSE TEST_SRC "${3DSceneModel_SOURCE_DIR}/src/Tests/*.cpp")

add_executable(tests ${TEST_SRC} ${UObj_HEADERS} ${IO_HEADERS})
target_link_libraries(tests Catch UrbanObject IO)
add_test(NAME test COMMAND tests -r junit)
