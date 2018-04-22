#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <vector>
#include <map>
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
         */
        class FileHandler
        {
        public:
            FileHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes);
            virtual ~FileHandler(void) = 0;
        protected:
            boost::filesystem::path filepath;
            std::map<std::string, bool> modes;
        };

        extern const std::vector<std::string> supported_scene_formats;

        std::string format(std::string const& input_format);
    }
}
