set(Shadow_SRC
    "${proj.city_SOURCE_DIR}/src/lib/shadow/bbox.cpp"
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
    "${proj.city_SOURCE_DIR}/src/lib/algorithms/util_algorithms.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/algorithms/test_utils.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/algorithms/scene_algorithms.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/algorithms/io_algorithms.cpp"    
)
set(Scene_SRC
    "${proj.city_SOURCE_DIR}/src/lib/scene/unode.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/scene/scene.cpp"
    "${proj.city_SOURCE_DIR}/src/lib/scene/utilities.cpp"
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
    "${proj.city_SOURCE_DIR}/src/lib/city.cpp"
)

# Shadow
add_library(city.shadow ${Shadow_SRC})
target_link_libraries(city.shadow ${LIBS})

# Scene
add_library(city.scene ${Scene_SRC})
target_link_libraries(city.scene city.shadow city.projection ${LIBS})

# Algorithms
add_library(city.algorithms ${Algorithms_SRC})
target_link_libraries(city.algorithms city.scene city.shadow city.io ${LIBS})

# Projections
add_library(city.projection ${Projection_SRC})
target_link_libraries(city.projection city.algorithms ${LIBS})
 
# Input/Output
add_library(city.io ${IO_SRC})
target_link_libraries(city.io city.shadow city.projection ${LIBS})

# city
add_library(proj.city ${CITY_SRC})
link_directories(proj.city ${LIBS_DIRS})
target_link_libraries(proj.city city.shadow city.io city.scene city.projection city.algorithms ${LIBS})
