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
    std::string filename("/home/ethiy/Workspace/3DSceneModel/ressources/3dModels/3DS/stealth.3ds");
    Lib3dsFile *file = lib3ds_file_load( filename.c_str() );

    std::cout << "Mesh version: " << file->mesh_version << std::endl << std::flush;
    std::cout << "Key f revision: " << file->keyf_revision << std::endl << std::flush;
    std::cout << "Name: " << file->name << std::endl << std::flush;
    std::cout << "Master scale: " << file->master_scale << std::endl << std::flush;
    std::cout << "Construction plane: " << to_string(file->construction_plane, 3) << std::endl << std::flush;
    std::cout << "Ambient: " << to_string(file->ambient, 3) << std::endl << std::flush;
    std::cout << "Meshes: " << sizeof file->meshes << std::endl << std::flush;
    std::cout << "Materials: " << sizeof file->materials << std::endl << std::flush;
    std::cout << "Cameras: " << sizeof file->cameras << std::endl << std::flush;
    std::cout << "Nodes: " << sizeof file->nodes << std::endl << std::flush;
    std::cout << "Lights: " << sizeof file->lights << std::endl << std::flush;
    std::cout << "Frames: " << file->frames << std::endl << std::flush;
    std::cout << "Segment from: " << file->segment_from << std::endl << std::flush;
    std::cout << "Segment to: " << file->segment_to << std::endl << std::flush;
    std::cout << "Current frame: " << file->current_frame << std::endl << std::flush;

    return EXIT_SUCCESS;
}