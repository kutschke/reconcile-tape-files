#include "SAMDataset.hh"

#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;

SAMDataset::SAMDataset():dsName("Unknown"){
}

// Process one line from the SAM listing file.
SAMDataset::SAMDataset ( const std::string& line, std::vector<std::string> const& deleted ){
  istringstream in(line);
  in >> location;
  in >> nRecords;
  in >> nFiles;
  in >> size;
  in >> nEvents;
  in >> dsName;

  isDeleted = std::find( deleted.begin(),  deleted.end(), dsName) != deleted.end();

}

// Add information for one enstore file.
void SAMDataset::addEnstoreFile( double size, std::string const& pnfs_path ) {
  ++nFoundFiles;
  nFoundSize += size;

  // Determine if the path is old or new style:
  //   old:  /pnfs/mu2e/phy-xxx
  //   new:  /pnfs/mu2e/tape/phy-xxx
  //   Similarly for usr-xxx
  auto i = pnfs_path.find("/mu2e/tape/");
  bool isNewStyle = ( i != string::npos );

  // Keep track if the whole dataset is old style, new style or mixed.
  // We do not expect any cases of both.
  PnfsLocation save = pnfs;
  if ( save == unknown ){
    if ( isNewStyle ){
      pnfs = newStyle;
    } else{
      pnfs = oldStyle;
    }
  } else if ( save == oldStyle ){
    if ( isNewStyle ){
      pnfs = both;
    }
  } else if ( save == newStyle ){
    if ( !isNewStyle ){
      pnfs = both;
    }
  } else if ( save != both ){
    cerr << "Error in assigning pnfs location .... " << pnfs << " " << isNewStyle << "  : " << pnfs_path << endl;
  }

}
