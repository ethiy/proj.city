#include <io/io_3ds.h>

#include <lib3ds/node.h>
#include <lib3ds/types.h>

#include <sstream>

namespace city
{
    namespace io
    {
        T3DSHandler::T3DSHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes)
            : FileHandler(_filepath, _modes)
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

        T3DSHandler::~T3DSHandler(void)
        {
           lib3ds_file_free(file);
        }

        scene::Scene T3DSHandler::get_scene(SceneTreeHandler const& scene_tree_file, bool from_xml)
        {
            if(from_xml)
            {
                auto building_ids = scene_tree_file.building_ids();
                std::vector<shadow::Mesh> building_meshes(building_ids.size());
                std::transform(
                    std::begin(building_ids),
                    std::end(building_ids),
                    std::begin(building_meshes),
                    [this](std::string const& building_id)
                    {
                        return mesh(building_id, std::set<char>{{'T', 'F'}});
                    }
                );
                return scene::Scene(
                    building_meshes,
                    mesh(
                        scene_tree_file.terrain_id(),
                        std::set<char>{'M'}
                    ).set_name("terrain"),
                    scene_tree_file.pivot(),
                    scene_tree_file.epsg_index()
                );
            }
            else
                return scene::Scene(
                    level_meshes(1, std::set<char>{{'T', 'F'}}),
                    level_terrain(1),
                    scene_tree_file.pivot(),
                    scene_tree_file.epsg_index()
                );
        }
        scene::Scene T3DSHandler::get_scene(void)
        {
            return scene::Scene(
                level_meshes(1, std::set<char>{{'T', 'F'}}),
                level_terrain(1)
            );
        }


        std::vector<city::shadow::Mesh> T3DSHandler::get_meshes(void)
        {
            std::ostringstream error_message;
            std::vector<city::shadow::Mesh> meshes;
            
            if (modes["read"])
            {
                Lib3dsMesh *p_meshes = file->meshes;
                while(p_meshes)
                {
                    meshes.push_back(city::shadow::Mesh(p_meshes));
                    p_meshes = p_meshes->next;
                }
            }
            else
            {
                error_message << std::boolalpha << "The read mode is set to:" << modes["read"] << "! You should set it as follows: \'modes[\"read\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }
            return meshes;
        }

        std::vector<shadow::Mesh> T3DSHandler::level_meshes(std::size_t const level, std::set<char> const& facet_types)
        {
            std::vector<std::string> nodes = get_nodes(level);
            std::vector<shadow::Mesh> meshes(nodes.size());
            std::transform(
                std::begin(nodes),
                std::end(nodes),
                std::begin(meshes),
                [this, facet_types](std::string const& node)
                {
                    return mesh(node, facet_types);
                }
            );
            return meshes;
        }
        shadow::Mesh T3DSHandler::level_terrain(std::size_t const level)
        {
            auto terrain_meshes = level_meshes(level, std::set<char>{'M'});
            return std::accumulate(
                std::begin(terrain_meshes),
                std::end(terrain_meshes),
                shadow::Mesh(),
                std::plus<shadow::Mesh>()
            ).set_name("terrain");
        }
        std::vector<std::vector<shadow::Mesh> > T3DSHandler::raw_level_meshes(std::size_t const level, std::set<char> const& facet_types)
        {
            std::vector<std::string> nodes = get_nodes(level);
            std::vector<std::vector<shadow::Mesh> > meshes(nodes.size());
            std::transform(
                std::begin(nodes),
                std::end(nodes),
                std::begin(meshes),
                [this, facet_types](std::string const& node)
                {
                    return node_meshes(node, facet_types);
                }
            );
            return meshes;
        }
        std::vector<shadow::Mesh> T3DSHandler::node_meshes(std::string const& node_name, std::set<char> const& facet_types)
        {
            auto meshes_by_type = node_meshes_by_type(node_name, facet_types);
            std::vector<shadow::Mesh> meshes(
                std::accumulate(
                    std::begin(meshes_by_type),
                    std::end(meshes_by_type),
                    std::size_t(0),
                    [](std::size_t const total, std::pair<char, std::deque<shadow::Mesh> > const& type_meshes)
                    {
                        return total + type_meshes.second.size();
                    }
                )
            );
            for(auto type_meshes : meshes_by_type)
                meshes.insert(std::end(meshes), std::begin(type_meshes.second), std::end(type_meshes.second));
            return meshes;
        }
        std::map<char, std::deque<shadow::Mesh> > T3DSHandler::node_meshes_by_type(std::string const& node_name, std::set<char> const& facet_types)
        {
            std::ostringstream error_message;
            std::map<char, std::deque<shadow::Mesh> > meshes;
            
            if (modes["read"])
            {
                Lib3dsNode *p_node = lib3ds_node_by_name(file->nodes, node_name.c_str(), LIB3DS_OBJECT_NODE);
                node_meshes(p_node, meshes, facet_types);
            }
            else
            {
                error_message << std::boolalpha << "The read mode is set to:" << modes["read"] << "! You should set it as follows: \'modes[\"read\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }
            return meshes;
        }
        shadow::Mesh T3DSHandler::mesh(std::string const& node_name, std::set<char> const& facet_types)
        {
            auto meshes = mesh_by_type(node_name, facet_types);

            return std::accumulate(
                std::begin(meshes),
                std::end(meshes),
                shadow::Mesh(),
                [](shadow::Mesh const& lhs, std::pair<char, shadow::Mesh> const& rhs)
                {
                    return lhs + rhs.second;
                }
            ).set_name(node_name);
        }
        std::map<char, shadow::Mesh> T3DSHandler::mesh_by_type(std::string const& node_name, std::set<char> const& facet_types)
        {
            auto meshes = node_meshes_by_type(node_name, facet_types);

            std::map<char, shadow::Mesh> mesh_by_type;

            for(auto const& pair_tm : meshes)
                mesh_by_type[pair_tm.first] = std::accumulate(
                    std::begin(pair_tm.second),
                    std::end(pair_tm.second),
                    shadow::Mesh()
                );
            return mesh_by_type;
        }

        std::vector<std::string> T3DSHandler::get_nodes(std::size_t const level)
        {
            std::deque<Lib3dsNode*> p_nodes{file->nodes};
            std::deque<Lib3dsNode*> childs;
            Lib3dsNode* q_buffer;
            std::size_t l(0);
            for(l = 0; l < level; l++)
                for(auto const node: p_nodes)
                    if(node->childs != nullptr)
                        for(q_buffer = node->childs; q_buffer != nullptr; q_buffer = q_buffer->next)
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

        void T3DSHandler::write_meshes(std::vector<city::shadow::Mesh> const& meshes)
        {
            std::ostringstream error_message;

            if (modes["write"])
            {
                file->meshes = meshes[0].to_3ds();
                Lib3dsMesh *current = file->meshes;
                std::for_each(
                    std::next(std::begin(meshes), 1),
                    std::end(meshes),
                    [&current](city::shadow::Mesh const& mesh) {
                        current->next = mesh.to_3ds();
                        current = current->next;
                    });
                current = nullptr;
                lib3ds_file_save(file, filepath.string().c_str());
            }
            else
            {
                error_message << std::boolalpha << "The write mode is set to:" << modes["write"] << "! You should set it as follows: \'modes[\"write\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }
        }

        void T3DSHandler::node_meshes(Lib3dsNode * node, std::map<char, std::deque<shadow::Mesh> > & meshes, std::set<char> const& facet_types)
        {
            Lib3dsNode * p_node;

            for(p_node=node->childs; p_node != nullptr; p_node = p_node->next)
                node_meshes(p_node, meshes, facet_types);
            
            Lib3dsMesh * mesh = lib3ds_file_mesh_by_name(file, node->name);
            auto type = mesh? facet_types.find(mesh->name[0]): std::end(facet_types);
            if(!mesh || type == std::end(facet_types) ) 
                return ;

            meshes[*type].push_back(city::shadow::Mesh(mesh));
        }
    }
}
