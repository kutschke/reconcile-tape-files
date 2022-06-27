#ifndef FileFamilies_hh
#define FileFamilies_hh

#include "FileFamily.hh"

#include <string>
#include <map>

struct FileFamilies {
public:

  FileFamilies (){}

  void addFile( std::string const& familyName, long long size );

  FileFamily const& get( std::string& familyName ){
    return families[familyName];
  }

  std::map<std::string,FileFamily> families;

};

#endif  // FileFamilies
