#pragma once

#include <shadow/point.h>

#include <string>
#include <vector>
#include <map>

namespace city
{
    template<typename T>
    std::vector<T> select(std::vector<T> const& container, std::map<std::size_t, std::size_t> & index_map)
    {
        std::vector<T> selected(index_map.size(), shadow::Point());
        
        for(auto const& index_pair : index_map)
            selected[index_pair.second] = container[index_pair.first];

        return selected;
    }


    shadow::Point line2pt(std::string const& line);
    shadow::Point str2pt(std::vector<double> & coordinates);
}
