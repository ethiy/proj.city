set(LIBS "")
set(LIBS_DIRS "")

# Find Lib3ds
include(cmake/modules/find_lib3ds.cmake)

include_directories(SYSTEM ${LIB3DS_INCLUDE_DIRS})
list(APPEND LIBS ${LIB3DS_LIBRARIES})

# Find Boost
FIND_PACKAGE(Boost REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
else(Boost_FOUND)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/local/boost_1_60_0/")
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/local/boost_1_60_0/lib64-msvc-14.0")
    Find_PACKAGE(Boost REQUIRED filesystem)
    if(Boost_FOUND)
        INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    endif(Boost_FOUND)
endif(Boost_FOUND)

set(Boost_USE_STATIC_LIBS        OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     OFF)
set(BOOST_ALL_DYN_LINK           OFF)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
list(APPEND LIBS ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})

# Find CGAL
FIND_PACKAGE(CGAL)
include( ${CGAL_USE_FILE} ) 
include_directories(${CGAL_INCLUDE_DIRS})
list(APPEND LIBS_DIRS ${CGAL_LIBRARIES_DIR})

# Find Catch
include(cmake/modules/catch.cmake)
enable_testing(true)
