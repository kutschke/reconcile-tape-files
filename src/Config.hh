#ifndef Config_hh
#define Config_hh

#include <string>

struct Config {

  enum Status { ok=0, unknown, help, bad_arguments, other };

  Config ( int argc, char** argv );

  Config ( std::string const& aenstoreDataFilename,
           std::string const& asamListingFilename,
           std::string const& adeletedDatasetsFilename,
           std::string const& aoutputDirectory):
    enstoreDataFilename(aenstoreDataFilename),
    samListingFilename(asamListingFilename),
    deletedDatasetsFilename(adeletedDatasetsFilename),
    outputDirectory(aoutputDirectory)
  {}


  std::string enstoreDataFilename;
  std::string samListingFilename;
  std::string deletedDatasetsFilename;
  std::string outputDirectory;
  long nHeartbeat  =   250000;
  long maxRecords  = 50000001;
  int  verbosity   =        0;
  Status status    =  unknown;
};

#endif
