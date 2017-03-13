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

# urban_shadow
file(GLOB_RECURSE Shadow_HEADERS "${3DSceneModel_SOURCE_DIR}/src/libs/shadow/*.h")
file(GLOB_RECURSE Shadow_SRC "${3DSceneModel_SOURCE_DIR}/src/libs/shadow/*.cpp")

add_library(urban_shadow ${Shadow_HEADERS} ${Shadow_SRC})
target_link_libraries(urban_shadow ${LIBS})

# Brick
file(GLOB_RECURSE Brick_HEADERS "${3DSceneModel_SOURCE_DIR}/src/libs/brick/*.h")
file(GLOB_RECURSE Brick_SRC "${3DSceneModel_SOURCE_DIR}/src/libs/brick/*.cpp")

add_library(urban_brick ${Brick_HEADERS} ${Brick_SRC} ${Shadow_HEADERS})
target_link_libraries(urban_brick urban_shadow ${LIBS})

# Projections
set(Projection_HEADER "${3DSceneModel_SOURCE_DIR}/src/libs/projection/projection.h")
file(GLOB_RECURSE Projection_SRC "${3DSceneModel_SOURCE_DIR}/src/libs/projection/*.cpp")

add_library(urban_projection ${Projection_HEADER} ${Projection_SRC} ${Brick_HEADERS} ${Algorithms_HEADER})
target_link_libraries(urban_projection urban_algorithms ${LIBS})

# Algorithms
set(Algorithms_HEADER "${3DSceneModel_SOURCE_DIR}/src/libs/algorithms/algorithms.h")
file(GLOB_RECURSE Algorithms_SRC "${3DSceneModel_SOURCE_DIR}/src/libs/algorithms/*.cpp")

add_library(urban_algorithms ${Algorithms_HEADER} ${Algorithms_SRC} ${Brick_HEADERS} ${Shadow_HEADERS} ${Projection_HEADER})
target_link_libraries(urban_algorithms urban_brick urban_shadow urban_projection ${LIBS})

# Input/Output
file(GLOB_RECURSE IO_HEADERS "${3DSceneModel_SOURCE_DIR}/src/libs/io/*.h")
file(GLOB_RECURSE IO_SRC "${3DSceneModel_SOURCE_DIR}/src/libs/io/*.cpp")

add_library(urban_io ${IO_HEADERS} ${IO_SRC} ${Shadow_HEADERS} ${Projection_HEADER})
target_link_libraries(urban_io urban_shadow urban_projection ${LIBS})

# urban
set(urban_HEADER "${3DSceneModel_SOURCE_DIR}/src/libs/urban.h")
add_library(urban SHARED ${urban_HEADER})
ImagineUseModules(urban Images)
link_directories(urban ${LIBS_DIRS})
target_link_libraries(urban urban_shadow urban_io urban_brick urban_projection urban_algorithms ${LIBS})

##
# ___________________________________________________ Samples ___________________________________________________
##

# Santa
set(SANTA_SRC "${3DSceneModel_SOURCE_DIR}/src/samples/hello_santa.cpp")
add_executable(hello_santa ${SANTA_SRC} ${Brick_HEADERS} ${IO_HEADERS} ${Shadow_HEADERS})
target_link_libraries(hello_santa urban)
link_directories(hello_santa ${LIBS_DIRS})

# Project Santa
set(Proj_XY_SANTA_SRC "${3DSceneModel_SOURCE_DIR}/src/samples/project_santa_xy.cpp")
add_executable(project_santa_xy ${Proj_XY_SANTA_SRC} ${Proj_HEADERS} ${Brick_HEADERS} ${IO_HEADERS} ${Algorithms_HEADER})
target_link_libraries(project_santa_xy urban)

##
# ____________________________________________________ Tests _____________________________________________________
##

file(GLOB_RECURSE TEST_SRC "${3DSceneModel_SOURCE_DIR}/src/tests/*.cpp")

add_executable(tests ${TEST_SRC} ${Shadow_HEADERS} ${IO_HEADERS} ${Brick_HEADERS} ${Projection_HEADER} ${Algorithms_HEADER})
target_link_libraries(tests Catch urban)
add_test(NAME test COMMAND tests -r junit)
