#include <algorithms/io_algorithms.h>

#include <sstream>

#include <iterator>
#include <algorithm>

#include <stdexcept>

namespace urban
{
    shadow::Point line2pt(std::string const& line)
    {
        std::istringstream buffer_line(line);

        std::vector<double> buffer;
        buffer.reserve(4);

        std::copy(
            std::istream_iterator<double>(buffer_line),
            std::istream_iterator<double>(),
            std::back_inserter(buffer)
        );

        return str2pt(buffer);
    }
    shadow::Point str2pt(std::vector<double> const& coordinates)
    {
        shadow::Point point;

         switch(coordinates.size())
        {
            case 3: 
                shadow::Point(coordinates[0], coordinates[1], coordinates[2]);
                break;
            case 4:
                static_cast<bool>(coordinates[4])
                    ? shadow::Point(coordinates[0] / coordinates[4], coordinates[1] / coordinates[4], coordinates[2] / coordinates[4])
                    : throw std::logic_error("Not implemented in this scope!");
                break;
            default:
                throw std::ios_base::failure("Unable to read points!");
        }

        return point;
    }
}
