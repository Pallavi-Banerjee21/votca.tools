#include "version.h"
#include "config.h"

namespace votca { namespace tools {

#ifdef HGVERSION
  static const std::string version_str = VERSION " " HGVERSION " (compiled " __DATE__ ", " __TIME__ ")";
#else
  static const std::string version_str = VERSION " (compiled " __DATE__ ", " __TIME__ ")";
#endif

const std::string &ToolsVersionStr()
{
    return version_str;
}

}}

