# Santa
set(SANTA_SRC "${3DSceneModel_SOURCE_DIR}/src/samples/hello_santa.cpp")
add_executable(hello_santa ${SANTA_SRC})
target_link_libraries(hello_santa urban)

# Project Santa
set(Proj_XY_SANTA_SRC "${3DSceneModel_SOURCE_DIR}/src/samples/project_santa_xy.cpp")
add_executable(project_santa_xy ${Proj_XY_SANTA_SRC})
target_link_libraries(project_santa_xy urban)
