# Lib3ds_FOUND
# Lib3ds_INCLUDE_DIR
# Lib3ds_LIBRARIES

find_path(Lib3ds_INCLUDE_DIR
    NAMES lib3ds/file.h
    HINTS /usr/include /usr/local/include
)
find_library(Lib3ds_LIBRARIES
    NAMES 3ds lib3ds
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Lib3ds DEFAULT_MSG Lib3ds_LIBRARIES Lib3ds_INCLUDE_DIR)

mark_as_advanced(Lib3ds_INCLUDE_DIR Lib3ds_LIBRARIES)
