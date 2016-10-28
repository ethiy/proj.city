
#include "3dslite/lib3dslite.h"

namespace sys = boost::filesystem;

int main( int, char**)
{
    unsigned short ERROR_CODE = EXIT_SUCCESS;
    sys::path filename("/home/ethiy/Workspace/3DSceneModel/ressources/3dModels/3DS/house/dom1 - kopia.3ds");//house/dom1 - kopia.3ds");
    lite3ds::Volume house( filename, ERROR_CODE);
    return ERROR_CODE;
}