#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <vector>
#include <map>
#include <string>

namespace city
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
            FileHandler(void) = delete;
            FileHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes);
            FileHandler(FileHandler && other);
            virtual ~FileHandler(void) = 0;

            FileHandler& operator=(FileHandler && other);
        protected:
            boost::filesystem::path filepath;
            std::map<std::string, bool> modes;
        };

        extern const std::vector<std::string> supported_scene_formats;

        std::string format(std::string const& input_format);
    }
}
