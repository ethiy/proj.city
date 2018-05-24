#pragma once

#include "config.h"

#include <shadow/mesh.h>

#include <io/io.h>
#include <io/io_off.h>
#include <io/io_obj.h>
#include <io/io_3ds.h>
#include <io/io_vector.h>
#include <io/io_raster.h>
#include <io/io_scene.h>

#include <scene/scene.h>

#include <projection/projection.h>

#include <algorithms/algorithms.h>

namespace city
{
    void print_possible_scene_formats(void);
}
