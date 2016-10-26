#include <cstdlib>

#include <string>
#include <array>
#include <iostream>
#include <algorithm>

#include <lib3ds/file.h>
#include <lib3ds/material.h>
#include <lib3ds/mesh.h>
#include <lib3ds/camera.h>
#include <lib3ds/light.h>

std::string f_to_string(float* v, size_t size)
{
    std::string s("( ");
    for( size_t it; it<size; ++it)
        s + ", " + std::to_string(v[it]);
    return s + ")";
}

int main( int, char**)
{
    std::string filename("/home/ethiy/Workspace/3DSceneModel/ressources/3dModels/3DS/house/dom1 - kopia.3ds");//watertower.3ds");
    Lib3dsFile *file = lib3ds_file_load( filename.c_str() );

    std::cout << "Mesh version: " << file->mesh_version << std::endl << std::flush;
    std::cout << "Key f revision: " << file->keyf_revision << std::endl << std::flush;
    std::cout << "Name: " << file->name << std::endl << std::flush;
    std::cout << "Master scale: " << file->master_scale << std::endl << std::flush;
    std::cout << "Construction plane: " << f_to_string(file->construction_plane, 3) << std::endl << std::flush;
    std::cout << "Ambient: " << f_to_string(file->ambient, 3) << std::endl << std::flush;

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
                                << "        Color: " << f_to_string(file->background.solid.col, 3) << std::endl 
                                << "    Gradient: " << std::endl
                                << "        Use: " << file->background.gradient.use << std::endl
                                << "        Percentage: " << file->background.gradient.percent << std::endl 
                                << "        Top: " << f_to_string(file->background.gradient.top, 3) << std::endl
                                << "        Middle: " << f_to_string(file->background.gradient.middle, 3) << std::endl 
                                << "        Bottom: " << f_to_string(file->background.gradient.bottom, 3) << std::endl
                                << "    Bitmap: " << std::endl 
                                << "        Use: " << file->background.bitmap.use << std::endl
                                << "        name: " << file->background.bitmap.name << std::endl 
                                << std::flush;

    std::cout << "Atmosphere:" << std::endl 
                                << "    Fog: " << std::endl 
                                << "        Use: " << file->atmosphere.fog.use << std::endl
                                << "        Color: " << f_to_string(file->atmosphere.fog.col, 3) << std::endl 
                                << "        Fog Backgound: " << file->atmosphere.fog.fog_background << std::endl
                                << "        Near Plane: " << file->atmosphere.fog.near_plane << std::endl 
                                << "        Near Density: " << file->atmosphere.fog.near_density << std::endl
                                << "        Far Plane: " << file->atmosphere.fog.far_plane << std::endl 
                                << "        Far Density: " << file->atmosphere.fog.far_density << std::endl
                                << "    Layer Fog: " << std::endl
                                << "        Use: " << file->atmosphere.layer_fog.use << std::endl
                                << "        Flag: " << file->atmosphere.layer_fog.flags << std::endl 
                                << "        Color: " << f_to_string(file->atmosphere.layer_fog.col, 3) << std::endl
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

    std::cout << "Viewport:" << std::endl 
                                << "    Layout: " << std::endl 
                                << "        Style: " << file->viewport.layout.style << std::endl
                                << "        Active: " << file->viewport.layout.active << std::endl
                                << "        Swap: " << file->viewport.layout.swap << std::endl
                                << "        Swap prior: " << file->viewport.layout.swap_prior << std::endl
                                << "        Swap view: " << file->viewport.layout.swap_view << std::endl
                                << "        Views: " << file->viewport.layout.views << std::endl
                                << "    Default view: " << std::endl
                                << "        Type: " << file->viewport.default_view.type << std::endl
                                << "        Position: " << f_to_string(file->viewport.default_view.position, 3) << std::endl
                                << "        Width: " << file->viewport.default_view.width << std::endl
                                << "        Horizontal angle: " << file->viewport.default_view.horiz_angle << std::endl
                                << "        Vertical angle: " << file->viewport.default_view.vert_angle << std::endl
                                << "        Roll angle: " << file->viewport.default_view.roll_angle << std::endl
                                << "        Camera: " << file->viewport.default_view.camera << std::endl
                                << std::flush;

    std::cout << "Viewport keyf:" << std::endl 
                                << "    Layout: " << std::endl 
                                << "        Style: " << file->viewport_keyf.layout.style << std::endl
                                << "        Active: " << file->viewport_keyf.layout.active << std::endl
                                << "        Swap: " << file->viewport_keyf.layout.swap << std::endl
                                << "        Swap prior: " << file->viewport_keyf.layout.swap_prior << std::endl
                                << "        Swap view: " << file->viewport_keyf.layout.swap_view << std::endl
                                << "        Views: " << file->viewport_keyf.layout.views << std::endl
                                << "    Default view: " << std::endl
                                << "        Type: " << file->viewport_keyf.default_view.type << std::endl
                                << "        Position: " << f_to_string(file->viewport_keyf.default_view.position, 3) << std::endl
                                << "        Width: " << file->viewport_keyf.default_view.width << std::endl
                                << "        Horizontal angle: " << file->viewport_keyf.default_view.horiz_angle << std::endl
                                << "        Vertical angle: " << file->viewport_keyf.default_view.vert_angle << std::endl
                                << "        Roll angle: " << file->viewport_keyf.default_view.roll_angle << std::endl
                                << "        Camera: " << file->viewport_keyf.default_view.camera << std::endl
                                << std::flush;

    std::cout << "Materials:" << std::endl << std::flush;
    size_t counter = 1;
    if(file->materials != NULL)
    {
        Lib3dsMaterial* m = file->materials;
        while(m->next != 0)
        {
            std::cout << "    " << counter++ << ": " << std::endl
                                        << "        name: " << m->name << std::endl
                                        << std::flush;
            m = m->next;
        }
    }

    std::cout << "Cameras:" << std::endl << std::flush;
    if(file->cameras != NULL)
    {
        counter = 1;
        Lib3dsCamera* cam = file->cameras;
        while(cam->next != NULL)
        {
            std::cout << "    " << counter++ << ": " << std::endl
                                        << "        name: " << cam->name << std::endl
                                        << std::flush;
            cam = cam->next;
        }
    }

    std::cout << "Lights:" << std::endl << std::flush;
    if(file->lights != NULL)
    {
        counter = 1;
        Lib3dsLight* lit = file->lights;
        while(lit->next != NULL)
        {
            std::cout << "    " << counter++ << ": " << std::endl
                                        << "        name: " << lit->name << std::endl
                                        << std::flush;
            lit = lit->next;
        }
    }

    std::cout << "Meshes:" << std::endl << std::flush;
    if(file->meshes != NULL)
    {
        counter = 1;
        Lib3dsMesh* moche = file->meshes;
        while(moche->next != NULL)
        {
            std::cout << "    " << counter++ << ": " << std::endl
                                        << "        name: " << moche->name << std::endl
                                        << std::flush;
            moche = moche->next;
        }
    }


    return EXIT_SUCCESS;
}