#include <algorithms/scene_algorithms.h>

namespace urban
{
    void save_building_duals(boost::filesystem::path const& root_path, urban::scene::Scene const& scene)
    {
        std::cout << "Saving brick duals... " << std::flush;
        boost::filesystem::path dual_dir(root_path / "dual_graphs");
        boost::filesystem::create_directory(dual_dir);
        for(auto const& building : scene)
        {
            std::fstream adjacency_file(
                boost::filesystem::path(dual_dir / (building.get_name() + ".txt")).string(),
                std::ios::out
            );
            urban::io::Adjacency_stream as(adjacency_file);
            as << building;
        }
        std::cout << " Done." << std::flush << std::endl;
    }

    std::vector<urban::projection::FootPrint> orthoproject(urban::scene::Scene const& scene)
    {
        std::cout << "Projecting... " << std::flush;
        std::vector<urban::projection::FootPrint> ortho_projections(scene.building_size());
        std::transform(
            std::begin(scene),
            std::end(scene),
            std::begin(ortho_projections),
            [](urban::scene::UNode const& building)
            {
                return urban::projection::FootPrint(building);
            }
        );
        std::cout << "Done." << std::flush << std::endl;

        return ortho_projections;
    }
}
