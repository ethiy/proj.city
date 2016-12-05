#include "io_3ds.h"

FileHandler<Lib3dsFile>::FileHandler(void){}

FileHandler<Lib3dsFile>::FileHandler(boost::filesystem::path _filepath, std::map<std::string, bool> _modes)
:filepath(_filepath), modes(_modes){}

FileHandler<Lib3dsFile>::~FileHandler(void){}

int FileHandler<Lib3dsFile>::read(std::vector<urban::Mesh> & meshes)
{
    try
    {
        if(modes["read"]==true)
        {
            if(boost::filesystem::is_regular_file(filepath))
            {
                file = lib3ds_file_load( filepath.string().c_str());
                Lib3dsMesh* p_meshes = file->meshes;
                while(p_meshes)
                {
                    meshes.push_back(urban::Mesh(*p_meshes));
                    p_meshes = p_meshes->next;
                }
            }
            else
            {
                boost::system::error_code ec(boost::system::errc::no_such_file_or_directory, boost::system::system_category());
                exit_code = boost::system::errc::no_such_file_or_directory;
                throw boost::filesystem::filesystem_error(ec.message(), ec);
            }
        }
        else
        {
            boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
            exit_code = boost::system::errc::io_error;
            throw boost::filesystem::filesystem_error(ec.message(), ec);
        }
    }
    catch (const boost::filesystem::filesystem_error& error)
    {
        std::cerr << "Reading " << filepath << " failed with: " << error.code().message() << std::endl;
    }
    return exit_code;
}

int FileHandler<Lib3dsFile>::write(std::vector<urban::Mesh> meshes)
{
    try
    {
        if(modes["write"] == true)
        {
            Lib3dsMesh* p_meshes = nullptr;// malloc !!
            Lib3dsMesh* cursor = p_meshes;
            std::for_each( std::begin(meshes), std::end(meshes), [&](urban::Mesh mesh)
                                                                    {
                                                                        cursor = mesh.to_3ds();
                                                                        cursor = cursor->next;
                                                                    }
                        );
            lib3ds_file_insert_mesh(file, p_meshes);
            lib3ds_file_save(file, filepath.string().c_str());
        }
        else
        {
            boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
            exit_code = boost::system::errc::io_error;
            throw boost::filesystem::filesystem_error(ec.message(), ec);
        }
    }
    catch (const boost::filesystem::filesystem_error & error)
    {
        
    }
    return exit_code;
}
