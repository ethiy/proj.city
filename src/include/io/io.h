#pragma once

#include <vector>
#include <string>

namespace urban
{
    /** @defgroup io Input/Output Structures
    *  This is a general declaration of file handler classes
    *  @{
    */

    /**
    *  namespace io is in four groups
    *  @see shadow @link shadow The interface to external structures @endlink
    *
    */
    namespace io
    {
        /**
         * @ingroup io
         * @brief FileHandler class representing a file handler.
         * @ tparam structure data structure representing 3D scene or a georeferenced projection
         */
        template<class structure>
        class FileHandler;

        extern const std::vector<std::string> supported_scene_formats;

        std::string format(std::string const& input_format);
    }
}
