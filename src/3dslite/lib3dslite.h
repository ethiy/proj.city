#ifndef LIB3DSLITE_H
#define LIB3DSLITE_H

#include <cstdlib>

#include <iostream>
#include <vector>

#include <boost/filesystem.hpp>

#include <lib3ds/mesh.h>
#include <lib3ds/file.h>


namespace sys = boost::filesystem;

namespace lite3ds
{
    class Volume
    {
    public:
        Volume(void);
        Volume(sys::path , unsigned short& );
        ~Volume(void);
    
    private:
        std::vector<Mesh> meshes;
    };
    
    inline Volume::Volume(void){}

    inline Volume::Volume(sys::path filepath, unsigned short& ERROR_CODE)
    {
        try
        {
            if( !sys::exists(filepath) )
                throw filepath;
        }
        catch (sys::path& images_path)
        {
            std::cerr << "[ERROR]:[" << images_path.string() << " does not exist]." << std::endl;
            ERROR_CODE = EXIT_FAILURE;
        }

        Lib3dsFile* file = lib3ds_file_load( filepath.string().c_str());
        
        // Loading meshes
        {
            meshes.push_back(file->meshes);
        } while(file->meshes->next);

    }

    inline Volume::~Volume(void){}
}

#endif
