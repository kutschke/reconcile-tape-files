#ifndef SAMDataset_hh
#define SAMDataset_hh

//
// Information about a single SAM data set.
// Most of the the information is read from the input file that contains the SAM listing.
// Other information is added when the listing of all enstore files is read.
//

#include <string>
#include <vector>

struct SAMDataset {
public:

  SAMDataset ();
  SAMDataset ( const std::string& line, std::vector<std::string> const& deleted );

  // Are files found in /pnfs/mu2e/file-family (old) or in /pnfs/mu2e/tape/file-family (new) or in both?
  // We do not expect any caes of both.
  enum PnfsLocation { unknown, oldStyle, newStyle, both };

  // Add information for one enstore file.
  void addEnstoreFile( double size, std::string const& pnfs_path );

  // Information read from the input file
  std::string location; // Location code: none, old style pnfs, new style pnfs, persistent, scratch
  long nRecords = 0;    // Number of SAM records
  long nFiles   = 0;    // Number of files recorded in SAM
  double size   = 0.;   // Size of all files in MB
  long nEvents  = 0;    // Number of events in these files
  std::string dsName;   // Dataset name

  // This information is added from the enstore file listing
  double       nFoundSize  = 0.;      // Total size of all files, found in the listing, in MB
  long         nFoundFiles = 0;       // Number of files found in the listing
  PnfsLocation pnfs        = unknown;

  // This is set by reading the list of deleted data sets, if it is provided.
  bool         isDeleted   = false;   // Is this one of the datasets that I deleted as part of the T10KC purge

};

#endif  // SAMDataset
