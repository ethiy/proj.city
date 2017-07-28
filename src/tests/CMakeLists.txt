file(GLOB_RECURSE TEST_SRC "${proj.city_SOURCE_DIR}/src/tests/*.cpp")

add_executable(tests ${TEST_SRC})
target_link_libraries(tests Catch urban)
