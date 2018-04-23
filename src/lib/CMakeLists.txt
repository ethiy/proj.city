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
    "${proj.city_SOURCE_DIR}/src/lib/algorithms/scene_algorithms.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/algorithms/io_algorithms.cpp"    
)
set(Scene_SRC
    "${proj.city_SOURCE_DIR}/src/lib/scene/unode.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/scene/scene.cpp"
)
set(IO_SRC
    "${proj.city_SOURCE_DIR}/src/lib/io/io.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/io/io_3ds.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/io/io_scene.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/io/io_scene_tree.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/io/io_off.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/io/io_obj.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/io/io_raster.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/io/io_vector.cpp"
)
set(CITY_SRC
    "${proj.city_SOURCE_DIR}/src/lib/urban.cpp"
)

# Shadow
add_library(city_shadow ${Shadow_SRC})
target_link_libraries(city_shadow ${LIBS})

# Scene
add_library(city_scene ${Scene_SRC})
target_link_libraries(city_scene city_shadow city_projection ${LIBS})

# Algorithms
add_library(city_algorithms ${Algorithms_SRC})
target_link_libraries(city_algorithms city_scene city_shadow city_io ${LIBS})

# Projections
add_library(city_projection ${Projection_SRC})
target_link_libraries(city_projection city_algorithms ${LIBS})
 
# Input/Output
add_library(city_io ${IO_SRC})
target_link_libraries(city_io city_shadow city_projection ${LIBS})

# city
add_library(proj.city SHARED ${CITY_SRC})
link_directories(proj.city ${LIBS_DIRS})
target_link_libraries(proj.city city_shadow city_io city_scene city_projection city_algorithms ${LIBS})
