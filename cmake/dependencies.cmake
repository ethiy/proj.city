set(LINKER_LIBS "")

include(cmake/modules/find_lib3ds.cmake)

include_directories(SYSTEM ${LIB3DS_INCLUDE_DIRS})
list(APPEND LINKER_LIBS ${LIB3DS_LIBRARIES})
