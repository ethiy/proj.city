#include <cstdlib>

#include <string>
#include <array>
#include <iostream>
#include <algorithm>

#include <lib3ds/file.h>

std::string to_string(float* v, size_t size)
{
    std::string s("( ");
    for( size_t it; it<size; ++it)
        s + ", " + std::to_string(v[it]);
    return s + ")";
}

int main( int, char**)
{
    std::string filename("/home/ethiy/Workspace/3DSceneModel/ressources/3dModels/3DS/house/dom1 - kopia.3ds");
    Lib3dsFile *file = lib3ds_file_load( filename.c_str() );

    std::cout << "Mesh version: " << file->mesh_version << std::endl << std::flush;
    std::cout << "Key f revision: " << file->keyf_revision << std::endl << std::flush;
    std::cout << "Name: " << file->name << std::endl << std::flush;
    std::cout << "Master scale: " << file->master_scale << std::endl << std::flush;
    std::cout << "Construction plane: " << to_string(file->construction_plane, 3) << std::endl << std::flush;
    std::cout << "Ambient: " << to_string(file->ambient, 3) << std::endl << std::flush;

    std::cout << "Frames: " << file->frames << std::endl << std::flush;
    std::cout << "Segment from: " << file->segment_from << std::endl << std::flush;
    std::cout << "Segment to: " << file->segment_to << std::endl << std::flush;
    std::cout << "Current frame: " << file->current_frame << std::endl << std::flush;

    std::cout << "Shadow: " << std::endl
                            << "    Map size: " << file->shadow.map_size << std::endl
                            << "    Low Bias: " << file->shadow.lo_bias << std::endl
                            << "    High Bias: " << file->shadow.hi_bias << std::endl
                            << "    Samples: " << file->shadow.samples << std::endl
                            << "    Range: " << file->shadow.range << std::endl
                            << "    Filter: " << file->shadow.filter << std::endl
                            << "    Ray Bias: " << file->shadow.ray_bias << std::endl
                            << std::flush;

    std::cout << "Background:" << std::endl 
                                << "    Solid: " << std::endl 
                                << "        Use: " << file->background.solid.use << std::endl
                                << "        Color: " << to_string(file->background.solid.col, 3) << std::endl 
                                << "    Gradient: " << std::endl
                                << "        Use: " << file->background.gradient.use << std::endl
                                << "        Percentage: " << file->background.gradient.percent << std::endl 
                                << "        Top: " << to_string(file->background.gradient.top, 3) << std::endl
                                << "        Middle: " << to_string(file->background.gradient.middle, 3) << std::endl 
                                << "        Bottom: " << to_string(file->background.gradient.bottom, 3) << std::endl
                                << "    Bitmap: " << std::endl 
                                << "        Use: " << file->background.bitmap.use << std::endl
                                << "        name: " << file->background.bitmap.name << std::endl 
                                << std::flush;

    std::cout << "Atmosphere:" << std::endl 
                                << "    Fog: " << std::endl 
                                << "        Use: " << file->atmosphere.fog.use << std::endl
                                << "        Color: " << to_string(file->atmosphere.fog.col, 3) << std::endl 
                                << "        Fog Backgound: " << file->atmosphere.fog.fog_background << std::endl
                                << "        Near Plane: " << file->atmosphere.fog.near_plane << std::endl 
                                << "        Near Density: " << file->atmosphere.fog.near_density << std::endl
                                << "        Far Plane: " << file->atmosphere.fog.far_plane << std::endl 
                                << "        Far Density: " << file->atmosphere.fog.far_density << std::endl
                                << "    Layer Fog: " << std::endl
                                << "        Use: " << file->atmosphere.layer_fog.use << std::endl
                                << "        Flag: " << file->atmosphere.layer_fog.flags << std::endl 
                                << "        Color: " << to_string(file->atmosphere.layer_fog.col, 3) << std::endl
                                << "        Near Y: " << file->atmosphere.layer_fog.near_y << std::endl
                                << "        Far Y: " << file->atmosphere.layer_fog.far_y << std::endl 
                                << "        Density: " << file->atmosphere.layer_fog.density << std::endl
                                << "    Distance Cue: " << std::endl 
                                << "        Use: " << file->atmosphere.dist_cue.use << std::endl
                                << "        Cue Background: " << file->atmosphere.dist_cue.cue_background << std::endl 
                                << "        Near Plane: " << file->atmosphere.dist_cue.near_plane << std::endl 
                                << "        Near Dimming: " << file->atmosphere.dist_cue.near_dimming << std::endl
                                << "        Far Plane: " << file->atmosphere.dist_cue.far_plane << std::endl 
                                << "        Far Dimming: " << file->atmosphere.dist_cue.far_dimming << std::endl
                                << std::flush;

    return EXIT_SUCCESS;
}