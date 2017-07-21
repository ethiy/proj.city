set(Proj_XY_CLI_SRC "${proj.city_SOURCE_DIR}/src/bin/orthoproject_scene.cpp")
add_executable(orthoproject_scene ${Proj_XY_CLI_SRC} ${urban_HEADER})
target_compile_options(orthoproject_scene PUBLIC -DVERSION=\"${PROJECT_VERSION}\")
target_link_libraries(orthoproject_scene urban)
