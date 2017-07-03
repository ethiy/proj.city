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
    "${proj.city_SOURCE_DIR}/src/libs/projection/Brick/brick_projection.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/projection/Camera/camera.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/projection/Face/face_projection.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/projection/Raster/raster_projection.cpp"
)
set(Algorithms_HEADER
    "${proj.city_SOURCE_DIR}/src/libs/algorithms/algorithms.h"
)
set(Algorithms_SRC
    "${proj.city_SOURCE_DIR}/src/libs/algorithms/brick/brick_algorithms.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/algorithms/building/building_algorithms.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/algorithms/ogr/ogr_algorithms.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/algorithms/raster/raster_algorithms.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/algorithms/projection/projection_algorithms.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/algorithms/shadow/shadow_algorithms.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/algorithms/utils/util_algorithms.cpp"
)
set(Scene_HEADERS
    "${proj.city_SOURCE_DIR}/src/libs/scene/brick/brick.h"
    "${proj.city_SOURCE_DIR}/src/libs/scene/building.h"
    "${proj.city_SOURCE_DIR}/src/libs/scene/scene.h"
)
set(Scene_SRC
    "${proj.city_SOURCE_DIR}/src/libs/scene/brick/brick.cpp"
    "${proj.city_SOURCE_DIR}/src/libs/scene/building.cpp"
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

# Shadow
add_library(urban_shadow ${Shadow_HEADERS} ${Shadow_SRC})
target_link_libraries(urban_shadow ${LIBS})

# Projections
add_library(urban_projection ${Projection_HEADER} ${Projection_SRC} ${Scene_HEADERS} ${Algorithms_HEADER})
target_link_libraries(urban_projection urban_algorithms ${LIBS})

# Algorithms
add_library(urban_algorithms ${Algorithms_HEADER} ${Algorithms_SRC} ${Scene_HEADERS} ${Shadow_HEADERS} ${Projection_HEADER})
target_link_libraries(urban_algorithms urban_scene urban_shadow urban_projection ${LIBS})

# Scene
add_library(urban_scene ${Scene_HEADERS} ${Scene_SRC} ${Shadow_HEADERS} ${Projection_HEADER} ${Algorithms_HEADER})
target_link_libraries(urban_scene urban_shadow urban_algorithms urban_projection ${LIBS})

# Input/Output
add_library(urban_io ${IO_HEADERS} ${IO_SRC} ${Shadow_HEADERS} ${Projection_HEADER} ${Line_HEADER} ${Adjacency_HEADER} ${Off_HEADER} ${Scene_HEADERS})
target_link_libraries(urban_io urban_shadow urban_projection ${LIBS})

# urban
set(urban_HEADER "${proj.city_SOURCE_DIR}/src/libs/urban.h")
add_library(urban SHARED ${urban_HEADER})
link_directories(urban ${LIBS_DIRS})
target_link_libraries(urban urban_shadow urban_io urban_scene urban_projection urban_algorithms ${LIBS})
