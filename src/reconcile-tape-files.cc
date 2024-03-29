//
// Main program to reconcile the enstore tape listing with the SAM dataset listing
//
// Inputs:
//   Enstore file listing
//      - This is a large file, too big for a browswer
//      - Get it using:  curl -O https://www-stken.fnal.gov/enstore/tape_inventory/COMPLETE_FILE_LISTING_mu2e
//   SAM directory listing from:  https://mu2e.fnal.gov/atwork/computing/ops/samMon.html
//   A file containaing the names of any deleted datasets
//
#include "Config.hh"
#include "EraClassifier.hh"
#include "FileFamilies.hh"
#include "printFileFamilySummary.hh"
#include "Record.hh"
#include "SAMListing.hh"
#include "splitString.hh"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


// Read the header lines from top of the enstore listing.
std::vector<std::string> parseHeaders ( std::ifstream& in ){
  std::string line;
  for ( int i=0; i<6; ++i){
    std::getline(in,line);
  }
  return splitString(line);
}

using namespace std;

int main( int argc, char** argv ){

  // Parse command line arguments.
  Config config(argc, argv);
  if ( config.status == Config::help ) {
    exit(0);
  }

  // Read the SAM listing; annotate any datasets deleted since the listing was made.
  SAMListing sam( config.samListingFilename, config.deletedDatasetsFilename, config.verbosity );
  cout << "Number of SAM datasets in listing: " << sam.size() <<  endl;

  // Info per file family for all files and excluding files marked for deletion.
  FileFamilies families_allFiles, families_currentFiles;

  // Datasets that are found in the Enstore listing but are missing from SAM.
  map<string,SAMDataset> missingDatasets;

  // Files in the ENSTORE listing for which I cannot determine dataset
  vector<string> filesWithNoDataset;

  // Summary of datasets by era.
  EraClassifier classifier;

  // Read the enstore listing and process it.
  ifstream in(config.enstoreDataFilename);
  std::vector<string> headers = parseHeaders(in);
  if ( config.verbosity > 1 ){
        int i=0;
        for ( auto const& s : headers ){
          cout << i << " "  << s.size() << "  " <<  s << endl;
          ++i;
        }
  }

  // Number of files on scrarch
  long nScratch{0};

  // Number of files that SFA packs
  long nAggregate{0};

  // Number of files that have an identifed dataset and that dataset is present in the SAM listing.
  // Excludes SFA packs.
  long nOK{0};

  // Size of all files from the enstore list, excluding the SFA packs (in bytes).
  long size{0};

  // Orphan files are those with an identified dataset but that dataset is not in the SAM listing.
  long nOrphan{0};
  double orphanSize{0};

  // Number of records read from the enstore listing
  long nRead{0};

  for ( long i=0; i<config.maxRecords; ++i ){
    string line;
    getline(in,line);
    if ( !in ) {
      break;
    }
    Record r(line);
    ++nRead;

    if ( i%config.nHeartbeat == 0 ){
      cout << " At record: " << i << endl;
    }

    if ( r.isAggregation ){
      ++nAggregate;
      continue;
    }

    size += r.size;

    families_allFiles.addFile( r.file_family, r.size );

    classifier.addFile(r);

    if ( r.isScratch ){
      ++nScratch;
      continue;
    }

    // Update per-dataset stats.
    auto ds = sam.find(r.dsname);
    if ( ds != sam.end() ){
      ++nOK;
      double size2 = double(r.size)/1000000;
      ds->second.addEnstoreFile(size2,r.pnfs_path);
      if ( ! ds->second.isDeleted ){
        families_currentFiles.addFile( r.file_family, r.size );
      }
    }else {
      ++nOrphan;
      orphanSize += r.size;
      SAMDataset& ds = missingDatasets[r.dsname];
      if ( r.dsname.empty() ) {
        filesWithNoDataset.push_back( r.pnfs_path );
      }
      ++ds.nFoundFiles;
      ds.nFoundSize += double(r.size)/1.e6;
      ds.dsName = r.dsname;
    }
  } // end loop over records in enstore listing

  long total = nOK + nOrphan + nScratch + nAggregate;

  // Cross-check file counts.
  long check{0};
  for ( auto const& ds : sam.datasets ){
    check += ds.second.nFoundFiles;
  }

  // Summary printout.
  cout << "Number with SAM dataset:               " << nOK        << endl;
  cout << "  -Cross-check:                        " << check      << "   "  << check-nOK  << endl;
  cout << "Number without SAM dataset:            " << nOrphan    << endl;
  cout << "Number on scratch:                     " << nScratch   << endl;
  cout << "Number of aggregates:                  " << nAggregate << endl;
  cout << "Total:                                 " << total      << endl;
  cout << "  -Cross-check:                        " << nRead      << endl;
  cout << endl;

  double size_MB = double(size)/1.e6;
  double size_TB = double(size)/1.e12;
  double orphanSize_TB =  double(orphanSize)/1.e12;
  cout << "Total size:               " << size_MB   << " MB" << endl;
  cout << "Total size:               " << size_TB   << " TB" << endl;
  cout << "Size without SAM dataset: " << orphanSize_TB << " TB" << endl;

  // Files missing from the SAM Summary when the dataset is present in SAM.
  long missingFiles{0};
  double missingSize{0};

  ofstream summary("data/SAM_Listing_with_pnfs_counts.txt");
  for ( auto const& i : sam.datasets ){
    SAMDataset const& ds = i.second;
    if ( ds.isOnTape() || ds.nFoundFiles > 0 ) {
      summary << setw(4)  << ds.location
              << setw(12) << ds.nRecords
              << setw(12) << ds.nFiles
              << setw(12) << long(ds.size)
              << setw(12) << ds.nEvents
              << setw(12) << ds.nFoundFiles
              << setw(12) << long(ds.nFoundSize)
              << setw(4)  << ds.isDeleted
              << setw(4)  << ds.pnfs
              << setw(12) << ds.nFoundFiles-ds.nFiles
              << setw(12) << long(std::round(ds.nFoundSize)-ds.size)
              << "   "
              << ds.dsName
              << endl;
      missingFiles += ds.nFoundFiles-ds.nFiles;
      missingSize  += ds.nFoundSize-ds.size;
    }
  }

  cout << endl;
  cout << "Number of files in ENSTORE but not SAM:  " << missingFiles      << endl;
  cout << "Size of files in ENSTORE but not SAM:    " << missingSize/1.e6  << " TB" << endl; // ds.size is in MB

  cout << endl;
  cout << "Number files with no dataset:  " << filesWithNoDataset.size() << endl;
  cout << "  - See data/Files_with_no_dataset.txt" << endl;

  ofstream summary2("data/SAM_Listing_missing_with_pnfs_counts.txt");
  for ( auto const& i : missingDatasets ){
    SAMDataset const& ds = i.second;
    string dsName = ( ds.dsName.empty() ) ? "No dataset name identified." : ds.dsName;
    summary2 << setw(4)  << ds.location
             << setw(12) << ds.nRecords
             << setw(12) << ds.nFiles
             << setw(12) << long(ds.size)
             << setw(12) << ds.nEvents
             << setw(12) << ds.nFoundFiles
             << setw(12) << long(ds.nFoundSize)
             << setw(4)  << ds.isDeleted
             << setw(4)  << ds.pnfs
             << setw(12) << ds.nFoundFiles-ds.nFiles
             << setw(12) << long(std::round(ds.nFoundSize-ds.size))
             << "   "
             << dsName
             << endl;
  }

  ofstream summary3("data/Files_with_no_dataset.txt");
  for ( string const& s : filesWithNoDataset ){
    summary3 << s << endl;
  }

  printFileFamilySummary( families_allFiles,     "data/fileFamilySummary_allFiles.txt" );
  printFileFamilySummary( families_currentFiles, "data/fileFamilySummary_currentFiles.txt" );

  long long sumFiles{0};
  double sumSize{0.};

  cout << endl;
  cout << "Summary of files by era: " << endl;
  cout << setw(12) << "Era"
       << setw(12) << "nFiles"
       << "    Size (TB)"
       << endl;
  for ( auto const& i : classifier.stats() ){
    double size_TB = double(i.second.size)/1.e12;
    cout << setw(12) << i.first
         << setw(12) << i.second.nFiles
         << "    " << size_TB
         << endl;
    sumFiles += i.second.nFiles;
    sumSize  += size_TB;
  }
  cout << "--------------------------------" << endl;
  cout << setw(12) << "Total: "
       << setw(12) << sumFiles
       << "     "  << sumSize
       << endl;

  exit(0);

}
