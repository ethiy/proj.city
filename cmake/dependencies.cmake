set(LIBS "")
set(LIBS_DIRS "")

# Find Lib3ds
include(cmake/modules/free3ds.cmake)

include_directories(SYSTEM ${LIB3DS_INCLUDE_DIRS})
list(APPEND LIBS ${LIB3DS_LIBRARIES})

# Find Boost
FIND_PACKAGE(Boost REQUIRED filesystem system)

set(Boost_USE_STATIC_LIBS        OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     OFF)
set(BOOST_ALL_DYN_LINK           OFF)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
list(APPEND LIBS ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})

# Find CGAL
FIND_PACKAGE(CGAL REQUIRED)
include( ${CGAL_USE_FILE} ) 
include_directories(${CGAL_INCLUDE_DIRS})
list(APPEND LIBS_DIRS ${CGAL_LIBRARIES_DIR})

# Find Python.h
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
list(APPEND LIBS ${PYTHON_LIBRARIES})

# Find GDAL
find_package(GDAL REQUIRED)
include_directories(${GDAL_INCLUDE_DIR})
list(APPEND LIBS ${GDAL_LIBRARY})

# Find Catch
include(cmake/modules/catch.cmake)
enable_testing(true)
