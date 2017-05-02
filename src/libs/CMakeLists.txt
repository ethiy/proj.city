# urban_shadow
set(Shadow_HEADERS
    "${3DSceneModel_SOURCE_DIR}/src/libs/shadow/Bbox/bbox.h"
    "${3DSceneModel_SOURCE_DIR}/src/libs/shadow/Face/face.h"
    "${3DSceneModel_SOURCE_DIR}/src/libs/shadow/Point/point.h"
    "${3DSceneModel_SOURCE_DIR}/src/libs/shadow/Vector/vector.h"
    "${3DSceneModel_SOURCE_DIR}/src/libs/shadow/mesh.h"
)
set(Shadow_SRC
    "${3DSceneModel_SOURCE_DIR}/src/libs/shadow/Bbox/bbox.cpp"
    "${3DSceneModel_SOURCE_DIR}/src/libs/shadow/Face/face.cpp"
    "${3DSceneModel_SOURCE_DIR}/src/libs/shadow/Point/point.cpp"
    "${3DSceneModel_SOURCE_DIR}/src/libs/shadow/Vector/vector.cpp"
    "${3DSceneModel_SOURCE_DIR}/src/libs/shadow/mesh.cpp"
)

add_library(urban_shadow ${Shadow_HEADERS} ${Shadow_SRC})
target_link_libraries(urban_shadow ${LIBS})

# Scene
set(Scene_HEADERS
    "${3DSceneModel_SOURCE_DIR}/src/libs/scene/brick/brick.h"
    "${3DSceneModel_SOURCE_DIR}/src/libs/scene/building.h"
    "${3DSceneModel_SOURCE_DIR}/src/libs/scene/scene.h"
)
set(Scene_SRC
    "${3DSceneModel_SOURCE_DIR}/src/libs/scene/brick/brick.cpp"
    "${3DSceneModel_SOURCE_DIR}/src/libs/scene/building.cpp"
    "${3DSceneModel_SOURCE_DIR}/src/libs/scene/scene.cpp"
)

add_library(urban_scene ${Scene_HEADERS} ${Scene_SRC} ${Shadow_HEADERS})
target_link_libraries(urban_scene urban_shadow ${LIBS})

# Projections
set(Projection_HEADER
    "${3DSceneModel_SOURCE_DIR}/src/libs/projection/projection.h"
)
set(Projection_SRC
    "${3DSceneModel_SOURCE_DIR}/src/libs/projection/Brick/brick_projection.cpp"
    "${3DSceneModel_SOURCE_DIR}/src/libs/projection/Camera/camera.cpp"
    "${3DSceneModel_SOURCE_DIR}/src/libs/projection/Face/face_projection.cpp"
    "${3DSceneModel_SOURCE_DIR}/src/libs/projection/Raster/raster_projection.cpp"
)

add_library(urban_projection ${Projection_HEADER} ${Projection_SRC} ${Scene_HEADERS} ${Algorithms_HEADER})
target_link_libraries(urban_projection urban_algorithms ${LIBS})

# Algorithms
set(Algorithms_HEADER
    "${3DSceneModel_SOURCE_DIR}/src/libs/algorithms/algorithms.h"
)
set(Algorithms_SRC
    "${3DSceneModel_SOURCE_DIR}/src/libs/algorithms/brick/brick_algorithms.cpp"
    "${3DSceneModel_SOURCE_DIR}/src/libs/algorithms/ogr/ogr_algorithms.cpp"
    "${3DSceneModel_SOURCE_DIR}/src/libs/algorithms/raster/raster_algorithms.cpp"
    "${3DSceneModel_SOURCE_DIR}/src/libs/algorithms/projection/projection_algorithms.cpp"
    "${3DSceneModel_SOURCE_DIR}/src/libs/algorithms/shadow/shadow_algorithms.cpp"
    "${3DSceneModel_SOURCE_DIR}/src/libs/algorithms/utils/util_algorithms.cpp"
)

add_library(urban_algorithms ${Algorithms_HEADER} ${Algorithms_SRC} ${Scene_HEADERS} ${Shadow_HEADERS} ${Projection_HEADER})
target_link_libraries(urban_algorithms urban_scene urban_shadow urban_projection ${LIBS})

# Input/Output
set(Line_HEADER
    "${3DSceneModel_SOURCE_DIR}/src/libs/io/Line/line.h"
)
set(Adjacency_HEADER
    "${3DSceneModel_SOURCE_DIR}/src/libs/io/Adjacency_stream/adjacency_stream.h"
)
set(Off_HEADER
    "${3DSceneModel_SOURCE_DIR}/src/libs/io/Off_stream/off_stream.h"
)
set(IO_HEADERS
    "${3DSceneModel_SOURCE_DIR}/src/libs/io/io.h"
    "${3DSceneModel_SOURCE_DIR}/src/libs/io/io_3ds.h"
    "${3DSceneModel_SOURCE_DIR}/src/libs/io/io_scene.h"
    "${3DSceneModel_SOURCE_DIR}/src/libs/io/io_off.h"
    "${3DSceneModel_SOURCE_DIR}/src/libs/io/io_gdal.h"
)
set(IO_SRC
    "${3DSceneModel_SOURCE_DIR}/src/libs/io/io_3ds.cpp"
    "${3DSceneModel_SOURCE_DIR}/src/libs/io/io_scene.cpp"
    "${3DSceneModel_SOURCE_DIR}/src/libs/io/io_off.cpp"
    "${3DSceneModel_SOURCE_DIR}/src/libs/io/io_gdal.cpp"
)

add_library(urban_io ${IO_HEADERS} ${IO_SRC} ${Shadow_HEADERS} ${Projection_HEADER} ${Line_HEADER} ${Adjacency_HEADER} ${Off_HEADER} ${Scene_HEADERS})
target_link_libraries(urban_io urban_shadow urban_projection ${LIBS})

# urban
set(urban_HEADER "${3DSceneModel_SOURCE_DIR}/src/libs/urban.h")
add_library(urban SHARED ${urban_HEADER})
link_directories(urban ${LIBS_DIRS})
target_link_libraries(urban urban_shadow urban_io urban_scene urban_projection urban_algorithms ${LIBS})
