#include <io/io.h>

namespace urban
{
    namespace io
    {
        FileHandler::FileHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes)
            : filepath(_filepath), modes(_modes)
        {}
        FileHandler::~FileHandler(void)
        {}
    }
}