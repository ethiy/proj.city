set(Proj_XY_CLI_SRC "${3DSceneModel_SOURCE_DIR}/src/cli/project_scene_xy.cpp")
add_executable(project_scene_xy ${Proj_XY_CLI_SRC} ${urban_HEADER})
target_link_libraries(project_scene_xy urban)
