set(orthoproject_SRC "${proj.city_SOURCE_DIR}/src/bin/orthoproject.cpp")
add_executable(orthoproject ${orthoproject_SRC})
target_link_libraries(orthoproject urban)
