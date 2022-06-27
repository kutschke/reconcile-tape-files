#include "FileFamilies.hh"

void FileFamilies::addFile( std::string const& familyName, long long size ){
  families[familyName].addFile(size);
}
