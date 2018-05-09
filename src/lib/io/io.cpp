#include <io/io.h>

namespace city
{
    namespace io
    {
        FileHandler::FileHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes)
            : filepath(_filepath), modes(_modes)
        {}
        FileHandler::FileHandler(FileHandler && other)
            : filepath(std::move(other.filepath)), modes(std::move(other.modes))
        {}
        FileHandler::~FileHandler(void)
        {}

        FileHandler& FileHandler::operator=(FileHandler && other)
        {
            filepath = std::move(other.filepath);
            modes = std::move(other.modes);

            return *this;
        }
    }
}
