#include <io/io_3ds.h>

#include <lib3ds/node.h>
#include <lib3ds/types.h>

#include <sstream>

namespace urban
{
    namespace io
    {
        FileHandler<Lib3dsFile>::FileHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes)
            : filepath(_filepath), modes(_modes) 
        {
            std::ostringstream error_message;

            if(modes["read"] && modes["write"])
            {
                boost::system::error_code ec(boost::system::errc::no_such_file_or_directory, boost::system::system_category());
                throw boost::filesystem::filesystem_error("Simultaneous reading and writing access is forbidden", ec);
            }

            if(!modes["read"] && !modes["write"])
            {
                boost::system::error_code ec(boost::system::errc::no_such_file_or_directory, boost::system::system_category());
                throw boost::filesystem::filesystem_error("You have to specify access type", ec);
            }

            if(modes["read"])
            {
                if(boost::filesystem::is_regular_file(filepath))
                    file = lib3ds_file_load(filepath.string().c_str());
                else
                {
                    error_message << "This file \"" << filepath.string() << "\" cannot be found! You should check the file path";
                    boost::system::error_code ec(boost::system::errc::no_such_file_or_directory, boost::system::system_category());
                    throw boost::filesystem::filesystem_error(error_message.str(), ec);
                }
            }
            if(modes["write"])
            {
                file = lib3ds_file_new();
            }
        }

        FileHandler<Lib3dsFile>::~FileHandler(void)
        {
           lib3ds_file_free(file);
        }


        std::vector<urban::shadow::Mesh> FileHandler<Lib3dsFile>::read(void) const
        {
            std::ostringstream error_message;
            std::vector<urban::shadow::Mesh> meshes;
            
            if (modes.at("read"))
            {
                Lib3dsMesh *p_meshes = file->meshes;
                while(p_meshes)
                {
                    meshes.push_back(urban::shadow::Mesh(p_meshes));
                    p_meshes = p_meshes->next;
                }
            }
            else
            {
                error_message << std::boolalpha << "The read mode is set to:" << modes.at("read") << "! You should set it as follows: \'modes[\"read\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }
            return meshes;
        }
        shadow::Mesh FileHandler<Lib3dsFile>::read(std::string const& node_name, std::set<char> const& facet_types) const
        {
            auto mesh_by_type = get_mesh_by_type(node_name, facet_types);

            return std::accumulate(
                std::begin(mesh_by_type),
                std::end(mesh_by_type),
                shadow::Mesh(),
                [](shadow::Mesh const& lhs, std::pair<char, shadow::Mesh> const& rhs)
                {
                    return lhs + rhs.second;
                }
            );
        }
        std::vector<shadow::Mesh> FileHandler<Lib3dsFile>::read(std::size_t const level, std::set<char> const& facet_types) const
        {
            std::vector<std::string> nodes = get_nodes(level);
            std::vector<shadow::Mesh> meshes(nodes.size());
            std::transform(
                std::begin(nodes),
                std::end(nodes),
                std::begin(meshes),
                [this, facet_types](std::string const& node)
                {
                    return read(node, facet_types);
                }
            );
            return meshes;
        }
        std::map<char, std::deque<shadow::Mesh> > FileHandler<Lib3dsFile>::get_meshes(std::string const& node_name, std::set<char> const& facet_types) const
        {
            std::ostringstream error_message;
            std::map<char, std::deque<shadow::Mesh> > meshes;
            
            if (modes.at("read"))
            {
                Lib3dsNode *p_node = lib3ds_node_by_name(file->nodes, node_name.c_str(), LIB3DS_OBJECT_NODE);
                node_meshes(p_node, meshes, facet_types);
            }
            else
            {
                error_message << std::boolalpha << "The read mode is set to:" << modes.at("read") << "! You should set it as follows: \'modes[\"read\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }
            return meshes;
        }
        std::map<char, shadow::Mesh> FileHandler<Lib3dsFile>::get_mesh_by_type(std::string const& node_name, std::set<char> const& facet_types) const
        {
            auto meshes = get_meshes(node_name, facet_types);

            std::map<char, shadow::Mesh> mesh_by_type;

            for(auto const& pair_tm : meshes)
                mesh_by_type[pair_tm.first] = std::accumulate(
                    std::begin(pair_tm.second),
                    std::end(pair_tm.second),
                    shadow::Mesh()
                );
            return mesh_by_type;
        }
        std::vector<std::string> FileHandler<Lib3dsFile>::get_nodes(std::size_t const level) const
        {
            std::deque<Lib3dsNode*> p_nodes{{file->nodes}};
            std::deque<Lib3dsNode*> childs;
            Lib3dsNode* q_buffer;
            std::size_t l(0);
            for(l = 0; l < level; l++)
                for(auto const node: p_nodes)
                    if(node->childs != NULL)
                        for(q_buffer = node->childs; q_buffer != NULL; q_buffer = q_buffer->next)
                            childs.push_back(q_buffer);
                p_nodes = childs;

            std::vector<std::string> nodes(p_nodes.size());
            if(l < level)
                throw std::runtime_error("The asked level is too deep for the scene tree!");
            else
                std::transform(
                    std::begin(p_nodes),
                    std::end(p_nodes),
                    std::begin(nodes),
                    [](Lib3dsNode const* p_node)
                    {
                        return std::string(p_node->name);
                    }
                );
            return nodes;
        }


        void FileHandler<Lib3dsFile>::write(std::vector<urban::shadow::Mesh> meshes)
        {
            std::ostringstream error_message;

            if (modes["write"])
            {
                file->meshes = meshes[0].to_3ds();
                Lib3dsMesh *current = file->meshes;
                std::for_each(
                    std::next(std::begin(meshes), 1),
                    std::end(meshes),
                    [&current](urban::shadow::Mesh const& mesh) {
                        current->next = mesh.to_3ds();
                        current = current->next;
                    });
                current = NULL;
                lib3ds_file_save(file, filepath.string().c_str());
            }
            else
            {
                error_message << std::boolalpha << "The write mode is set to:" << modes["write"] << "! You should set it as follows: \'modes[\"write\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }
        }

        void FileHandler<Lib3dsFile>::node_meshes(Lib3dsNode * node, std::map<char, std::deque<shadow::Mesh> > & meshes, std::set<char> const& facet_types) const
        {
            Lib3dsNode * p_node;

            for(p_node=node->childs; p_node != NULL; p_node = p_node->next)
                node_meshes(p_node, meshes, facet_types);
            
            Lib3dsMesh * mesh = lib3ds_file_mesh_by_name(file, node->name);
            auto type = mesh? facet_types.find(mesh->name[0]): std::end(facet_types);
            if(!mesh || type == std::end(facet_types) ) 
                return ;

            meshes[*type].push_back(urban::shadow::Mesh(mesh));
        }
    }
}
