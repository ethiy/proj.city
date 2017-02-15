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
file(GLOB_RECURSE Shadow_HEADERS "${3DSceneModel_SOURCE_DIR}/src/Shadow/*.h")
file(GLOB_RECURSE Shadow_SRC "${3DSceneModel_SOURCE_DIR}/src/Shadow/*.cpp")

add_library(Shadow ${Shadow_HEADERS} ${Shadow_SRC})
target_link_libraries(Shadow ${LIBS})

# UrbanObject
file(GLOB_RECURSE UObj_HEADERS "${3DSceneModel_SOURCE_DIR}/src/UrbanObject/*.h")
file(GLOB_RECURSE UObj_SRC "${3DSceneModel_SOURCE_DIR}/src/UrbanObject/*.cpp")

add_library(UrbanObject ${UObj_HEADERS} ${UObj_SRC} ${Shadow_HEADERS})
target_link_libraries(UrbanObject Shadow ${LIBS})

# Projections
file(GLOB_RECURSE Projection_HEADERS "${3DSceneModel_SOURCE_DIR}/src/Projection/*.h")
file(GLOB_RECURSE Projection_SRC "${3DSceneModel_SOURCE_DIR}/src/Projection/*.cpp")

add_library(Projection ${Projection_HEADERS} ${Projection_SRC} ${UObj_HEADERS})

# Algorithms
file(GLOB_RECURSE Algorithms_HEADERS "${3DSceneModel_SOURCE_DIR}/src/Algorithms/*.h")
file(GLOB_RECURSE Algorithms_SRC "${3DSceneModel_SOURCE_DIR}/src/Algorithms/*.cpp")

add_library(Algorithms ${Algorithms_HEADERS} ${Algorithms_SRC} ${UObj_HEADERS} ${Shadow_HEADERS} ${Projection_HEADERS})
target_link_libraries(Algorithms UrbanObject Shadow Projection ${LIBS})

# Input/Output
file(GLOB_RECURSE Line_HEADERS "${3DSceneModel_SOURCE_DIR}/src/IO/Line/*.h")
file(GLOB_RECURSE Line_SRC "${3DSceneModel_SOURCE_DIR}/src/IO/Line/*.cpp")

add_library(Line ${Line_HEADERS} ${Line_SRC})
target_link_libraries(Line ${LIBS})

file(GLOB_RECURSE IO_HEADERS "${3DSceneModel_SOURCE_DIR}/src/IO/*.h")
file(GLOB_RECURSE IO_SRC "${3DSceneModel_SOURCE_DIR}/src/IO/*.cpp")

add_library(IO ${IO_HEADERS} ${IO_SRC} ${Shadow_HEADERS} ${Line_HEADERS})
target_link_libraries(IO Shadow Line ${LIBS})

add_library(urban SHARED ${Shadow_HEADERS} ${Line_HEADERS} ${IO_HEADERS} ${UObj_HEADERS} ${Projection_HEADERS} ${Algorithms_HEADERS})
target_link_libraries(urban Shadow Line IO UrbanObject Projection Algorithms ${LIBS})

##
# ___________________________________________________ Samples ___________________________________________________
##

# Santa
set(SANTA_SRC "${3DSceneModel_SOURCE_DIR}/src/Samples/hello_santa.cpp")
add_executable(hello_santa ${SANTA_SRC} ${UObj_HEADERS} ${IO_HEADERS})
target_link_libraries(hello_santa urban)
link_directories(hello_santa ${LIBS_DIRS})

# Project Santa
set(Proj_XY_SANTA_SRC "${3DSceneModel_SOURCE_DIR}/src/Samples/project_santa_xy.cpp")
add_executable(project_santa_xy ${Proj_HEADERS} ${Proj_XY_SANTA_SRC} ${UObj_HEADERS} ${IO_HEADERS} ${Algorithms_HEADERS})
target_link_libraries(project_santa_xy urban)
link_directories(project_santa_xy ${LIBS_DIRS})

##
# ____________________________________________________ Tests _____________________________________________________
##

file(GLOB_RECURSE TEST_SRC "${3DSceneModel_SOURCE_DIR}/src/Tests/*.cpp")

add_executable(tests ${TEST_SRC} ${Shadow_HEADERS} ${Line_HEADERS} ${IO_HEADERS} ${UObj_HEADERS} ${Projection_HEADERS} ${Algorithms_HEADERS})
target_link_libraries(tests Catch urban)
add_test(NAME test COMMAND tests -r junit)
