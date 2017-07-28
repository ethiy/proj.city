set(Shadow_HEADERS
    "${proj.city_SOURCE_DIR}/src/libs/shadow/Bbox/bbox.h"
    "${proj.city_SOURCE_DIR}/src/libs/shadow/Face/face.h"
    "${proj.city_SOURCE_DIR}/src/libs/shadow/Point/point.h"
    "${proj.city_SOURCE_DIR}/src/libs/shadow/Vector/vector.h"
    "${proj.city_SOURCE_DIR}/src/libs/shadow/mesh.h"
)
set(Shadow_SRC
    "${proj.city_SOURCE_DIR}/src/libs/shadow/Bbox/bbox.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/shadow/Face/face.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/shadow/Point/point.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/shadow/Vector/vector.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/shadow/mesh.cpp"
)
set(Projection_HEADER
    "${proj.city_SOURCE_DIR}/src/libs/projection/projection.h"
)
set(Projection_SRC
    "${proj.city_SOURCE_DIR}/src/libs/projection/scene/brick_projection.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/projection/scene/scene_projection.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/projection/camera/camera.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/projection/face/face_projection.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/projection/raster/raster_projection.cpp"
)
set(Algorithms_HEADER
    "${proj.city_SOURCE_DIR}/src/libs/algorithms/algorithms.h"
)
set(Algorithms_SRC
    "${proj.city_SOURCE_DIR}/src/libs/algorithms/scene/unode_algorithms.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/algorithms/ogr/ogr_algorithms.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/algorithms/utils/util_algorithms.cpp"
)
set(Scene_HEADERS
    "${proj.city_SOURCE_DIR}/src/libs/scene/unode.h"
    "${proj.city_SOURCE_DIR}/src/libs/scene/scene.h"
)
set(Scene_SRC
    "${proj.city_SOURCE_DIR}/src/libs/scene/unode.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/scene/scene.cpp"
)
set(Line_HEADER
    "${proj.city_SOURCE_DIR}/src/libs/io/Line/line.h"
)
set(Adjacency_HEADER
    "${proj.city_SOURCE_DIR}/src/libs/io/Adjacency_stream/adjacency_stream.h"
)
set(Off_HEADER
    "${proj.city_SOURCE_DIR}/src/libs/io/Off_stream/off_stream.h"
)
set(IO_HEADERS
    "${proj.city_SOURCE_DIR}/src/libs/io/io.h"
    "${proj.city_SOURCE_DIR}/src/libs/io/io_3ds.h"
    "${proj.city_SOURCE_DIR}/src/libs/io/io_scene.h"
    "${proj.city_SOURCE_DIR}/src/libs/io/io_off.h"
    "${proj.city_SOURCE_DIR}/src/libs/io/io_gdal.h"
)
set(IO_SRC
    "${proj.city_SOURCE_DIR}/src/libs/io/io_3ds.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/io/io_scene.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/io/io_off.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/io/io_gdal.cpp"
)
set(urban_HEADER "${3DSceneModel_SOURCE_DIR}/src/libs/urban.h")

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
