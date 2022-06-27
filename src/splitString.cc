#include "splitString.hh"

// Split string into space separated fields; ignore leading and trailing spaces.
// tabs are treated like any other character, not as equivalent to spaces.

std::vector<std::string> splitString( std::string const& line ){
  std::vector<std::string> v;

  // Strip any leading spaces.
  std::size_t i=0;
  while (i <line.size() ){
    if ( line[i] != ' ' ) break;
    ++i;
  }

  while (i < line.size() ){
    std::size_t j = line.substr(i).find(' ');

    // Multiple spaces count as one.
    if ( j == 0 ){
      ++i;
      continue;
    }

    v.emplace_back(( j == std::string::npos ) ? line.substr(i) : line.substr(i,j) );

    if ( j == std::string::npos ) {
      break;
    }
    i = i+j+1;

  }

  return v;
}
