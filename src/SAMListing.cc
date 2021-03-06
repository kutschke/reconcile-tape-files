#include "SAMListing.hh"
#include "readDeletedDatasets.hh"
#include "splitString.hh"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace {
  void positionAfterHeader( ifstream& in ){
    while (in){
      string line;
      getline(in,line);
      if ( !in ) {
        throw std::logic_error("End of file searching for header line in SAM listing input file.");
      }
      if ( line.find("location records") == 0 ){
        break;
      }
    }
  }

}

SAMListing::SAMListing ( std::string const& inputFile, string const& deletedDatasetFile, int verbosity ){

  vector<string> deleted = readDeletedDatasets( deletedDatasetFile );
  if ( verbosity > 0 ) cout << "Number of deleted datasets read from list: " << deleted.size() << endl;

  ifstream in(inputFile);
  positionAfterHeader(in);
  while (in){
    string line;
    getline(in,line);
    if ( !in ) {
      throw std::logic_error("Unexpected early end of file while reading SAM listing input file.");
    }

    // Find totals line that follows the list of data sets.
    std::vector<std::string> vs = splitString( line );
    if ( vs.size() != 6 ){
      if ( verbosity > 0 ) cout << "Totals line from the SAM Listing: " << vs.size() << line << endl;
      break;
    }

    SAMDataset ds(line,deleted);
    datasets[ds.dsName] = ds;
  }

}
