#ifndef splitString_hh
#define splitString_hh

#include <string>
#include <vector>

// Split string into space separated fields; ignore leading and trailing spaces.
// tabs are treated like any other character, not as equivalent to spaces.
std::vector<std::string> splitString( std::string const& line );

#endif

