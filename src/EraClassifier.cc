#include "EraClassifier.hh"
#include "Record.hh"

#include <algorithm>

void EraClassifier::Stats::addFile ( long long asize) {
  ++nFiles;
  size += asize;
}

void EraClassifier::addFile ( Record const& r ){

  // Do the comparison caseblind.
  std::string name = r.pnfs_path;
  std::transform( name.begin(), name.end(), name.begin(), ::tolower);

  if ( name.find("cd3") != std::string::npos ){
    _stats["cd3"].addFile( r.size );
  } else if ( name.find("mdc2020") != std::string::npos){
    _stats["mdc2020"].addFile( r.size );
  } else if ( name.find("mdc2018") != std::string::npos){
    _stats["mdc2018"].addFile( r.size );
  }  else if ( name.find("su2020") != std::string::npos){
    _stats["su2020"].addFile( r.size );
  }  else if ( name.find(".mu2e.") != std::string::npos){
    _stats[".mu2e."].addFile( r.size );
  } else{
    _stats["other"].addFile( r.size );
  }

}
