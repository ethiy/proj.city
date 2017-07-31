set(Proj_XY_CLI_SRC "${proj.city_SOURCE_DIR}/src/bin/orthoproject_scene.cpp")
add_executable(orthoproject_scene ${Proj_XY_CLI_SRC})
target_link_libraries(orthoproject_scene urban)
