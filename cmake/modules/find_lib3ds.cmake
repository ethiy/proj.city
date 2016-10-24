# find lib3ds includes and library
#
# LIB3DS_INCLUDE_DIRS - where the lib3ds directory containing the headers can be
#                      found
# LIB3DS_LIBRARIES     - full path to the lib3ds library
# LIB3DS_FOUND       - true if lib3ds was found

find_path(LIB3DS_INCLUDE_DIRS lib3ds/file.h
    /usr/include
    /usr/local/include
    $ENV{INCLUDE}
)
find_library(LIB3DS_LIBRARIES NAMES 3ds lib3ds)

if(LIB3DS_INCLUDE_DIRS)
    message(STATUS "Found lib3ds include dir: ${LIB3DS_INCLUDE_DIRS}")
else(LIB3DS_INCLUDE_DIRS)
    message(STATUS "Could NOT find lib3ds headers.")
endif(LIB3DS_INCLUDE_DIRS)

if(LIB3DS_LIBRARIES)
    message(STATUS "Found lib3ds library: ${LIB3DS_LIBRARIES}")
else(LIB3DS_LIBRARIES)
    message(STATUS "Could NOT find lib3ds library.")
endif(LIB3DS_LIBRARIES)


if(LIB3DS_INCLUDE_DIRS and LIB3DS_LIBRARIES)
    set(LIB3DS_FOUND true)
else(LIB3DS_INCLUDE_DIRS and LIB3DS_LIBRARIES)
    set(LIB3DS_FOUND FALSE)
    if(Lib3ds_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find lib3ds. Please install lib3ds (http://lib3ds.sourceforge.net)")
    endif(Lib3ds_FIND_REQUIRED)
endif(LIB3DS_INCLUDE_DIRS and LIB3DS_LIBRARIES)
