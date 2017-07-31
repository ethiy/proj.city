set(Shadow_SRC
    "${proj.city_SOURCE_DIR}/src/lib/shadow/bbox.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/shadow/utilities.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/shadow/face.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/shadow/point.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/shadow/vector.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/shadow/mesh.cpp"
)
set(Projection_SRC
    "${proj.city_SOURCE_DIR}/src/lib/projection/brick_projection.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/projection/scene_projection.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/projection/camera.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/projection/face_projection.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/projection/raster_projection.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/projection/utilities.cpp"
)
set(Algorithms_SRC
    "${proj.city_SOURCE_DIR}/src/lib/algorithms/unode_algorithms.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/algorithms/util_algorithms.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/algorithms/test_utils.cpp"
)
set(Scene_SRC
    "${proj.city_SOURCE_DIR}/src/lib/scene/unode.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/scene/scene.cpp"
)
set(IO_SRC
    "${proj.city_SOURCE_DIR}/src/lib/io/io_3ds.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/io/io_scene.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/io/io_off.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/io/io_gdal.cpp"
)

# Shadow
add_library(urban_shadow ${Shadow_SRC})
target_link_libraries(urban_shadow ${LIBS})

# Scene
add_library(urban_scene ${Scene_SRC})
target_link_libraries(urban_scene urban_shadow urban_projection ${LIBS})

# Algorithms
add_library(urban_algorithms ${Algorithms_SRC})
target_link_libraries(urban_algorithms urban_scene urban_shadow ${LIBS})

# Projections
add_library(urban_projection ${Projection_SRC})
target_link_libraries(urban_projection urban_algorithms ${LIBS})
 
# Input/Output
add_library(urban_io ${IO_SRC})
target_link_libraries(urban_io urban_shadow urban_projection ${LIBS})

# urban
add_library(urban STATIC)
link_directories(urban ${LIBS_DIRS})
target_link_libraries(urban urban_shadow urban_io urban_scene urban_projection urban_algorithms ${LIBS})
