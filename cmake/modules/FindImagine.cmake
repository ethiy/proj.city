# Imagine_FOUND
# Imagine_INCLUDE_DIR
# Imagine_LIBRARIES

find_path(Imagine_INCLUDE_DIR
    NAMES Imagine/Common.h Imagine/Images.h Imagine/Graphics.h Imagine/LinAlg.h
    HINTS /usr/include /usr/local/include
)
find_library(Imagine_LIBRARIES
    NAMES ImagineGraphics ImagineLinAlg
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Imagine DEFAULT_MSG Imagine_LIBRARIES Imagine_INCLUDE_DIR)

mark_as_advanced(Imagine_INCLUDE_DIR Imagine_LIBRARIES)
