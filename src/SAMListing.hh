#ifndef SAMListing_hh
#define SAMListing_hh

//
// Read the listing of SAM datasets and store in memory.
//
// If the list of deleted datasets is provided then read that too and annotate the SAM listing stored in memory.
//  - This allows us mark datasets and completed after we have deleted them; otherwise we would neded to
//    regenerate the SAM dataset listing which is expensive.
//

#include "SAMDataset.hh"

#include <map>
#include <string>

struct SAMListing {
public:

  SAMListing ( std::string const& inputFile, std::string const& deletedDatasetFile, int verbosity=0 );

  size_t size() const { return datasets.size(); }
  auto find( std::string const& dsname ) { return datasets.find(dsname); }
  auto end() { return datasets.end(); }

  std::map<std::string,SAMDataset> datasets;

};

#endif  // SAMListing
