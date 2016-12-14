#include "io_3ds.h"

namespace urban
{
    namespace io
    {
        FileHandler<Lib3dsFile>::FileHandler(void) {}

        FileHandler<Lib3dsFile>::FileHandler(boost::filesystem::path _filepath, std::map<std::string, bool> _modes)
            : filepath(_filepath), modes(_modes) {}

        FileHandler<Lib3dsFile>::~FileHandler(void) {}

        std::vector<urban::ShadowMesh> FileHandler<Lib3dsFile>::read(void)
        {
            std::vector<urban::ShadowMesh> meshes;
            if (modes["read"])
            {
                if (boost::filesystem::is_regular_file(filepath))
                {
                    file = lib3ds_file_load(filepath.string().c_str());
                    Lib3dsMesh *p_meshes = file->meshes;
                    while (p_meshes)
                    {
                        meshes.push_back(urban::ShadowMesh(*p_meshes));
                        p_meshes = p_meshes->next;
                    }
                }
                else
                {
                    boost::system::error_code ec(boost::system::errc::no_such_file_or_directory, boost::system::system_category());
                    throw boost::filesystem::filesystem_error(ec.message(), ec);
                }
            }
            else
            {
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(ec.message(), ec);
            }
            return meshes;
        }

        void FileHandler<Lib3dsFile>::write(std::vector<urban::ShadowMesh> meshes)
        {
            if (modes["write"])
            {
                file->meshes = reinterpret_cast<Lib3dsMesh *>(calloc(sizeof(Lib3dsMesh), 1));
                Lib3dsMesh *current = file->meshes;
                std::for_each(
                    std::begin(meshes),
                    std::end(meshes),
                    [&](urban::ShadowMesh mesh) {
                        current = mesh.to_3ds();
                        current->next = reinterpret_cast<Lib3dsMesh *>(calloc(sizeof(Lib3dsMesh), 1));
                        current = current->next;
                    });
                lib3ds_file_save(file, filepath.string().c_str());
            }
            else
            {
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(ec.message(), ec);
            }
        }
    }
}
