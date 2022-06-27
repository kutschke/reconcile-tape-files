#include "Config.hh"

// See https://theboostcpplibraries.com/boost.program_options
#include <boost/program_options.hpp>

#include <iostream>
#include <string>

namespace po = boost::program_options;

// Helper functions
namespace {
  void usage(){
    std::cout << "usage: analyze-tape-files --enstore-data <filename> --SAM-listing <filename> [--deleted-datasets <filename>] --output-directory <directory>" << std::endl;
  }

  void check_required_arguments( po::variables_map const& vm ){
    bool ok = true;
    if ( vm.count("enstore-data") == 0 ){
      std::cerr << "error: missing --enstore-data <filename> " << std::endl;
      ok = false;
    }
    if ( vm.count("SAM-listing") == 0 ){
      std::cerr << "error: missing --SAM-listing <filename> " << std::endl;
      ok = false;
    }
    if ( ! ok ){
      throw std::logic_error ("error checking the command line arguments.");
    }
  }
}

// process command line options and set the data members.
Config::Config( int argc, char** argv ){

  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "Print this description:")
    ("enstore-data,e", po::value<std::string>(),"Input file containing enstore data (required)")
    ("SAM-listing,s", po::value<std::string>(),"Input file containing SAM dataset listing (required)")
    ("deleted-datasets,d", po::value<std::string>(),"Input file containing names of deleted datasets (optional)")
    ("output-directory,o", po::value<std::string>()->default_value("."),"Directory for output files (optional)")
    ("n-heartbeat,n", po::value<long>()->default_value(nHeartbeat),"Number of records between heartbeat printout (optional)")
    ("max-records,m", po::value<long>()->default_value(maxRecords),"Maximum number or Number of records to read from Enstore file (optional)")
    ("verbosity,v", po::value<int>()->default_value(verbosity),"Verbosity (optional)")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if ( vm.count("help") ){
    usage();
    std::cout << desc << std::endl;
    status = help;
    return;
  }

  check_required_arguments(vm);

  // Set data members.
  enstoreDataFilename      = vm["enstore-data"].as<std::string>();
  samListingFilename       = vm["SAM-listing"].as<std::string>();
  outputDirectory          = vm["output-directory"].as<std::string>();

  if ( vm.count("deleted-datasets") ){
    deletedDatasetsFilename = vm["deleted-datasets"].as<std::string>();
  }

  if ( vm.count("n-heartbeat") ){
    nHeartbeat = vm["n-heartbeat"].as<long>();
  }

  if ( vm.count("max-records") ){
    maxRecords = vm["max-records"].as<long>();
  }

  if ( vm.count("verbosity") ){
    verbosity = vm["verbosity"].as<int>();
  }

  status = ok;

  if ( verbosity > 0 ) {
    std::cout << "Enstore data filename:     " << enstoreDataFilename << std::endl;
    std::cout << "SAM listing filename:      " << samListingFilename  << std::endl;
    std::cout << "Output directory:          " << outputDirectory     << std::endl;
    if ( vm.count("deleted-datasets") ){
      std::cout << "Deleted datasets filename: " << deletedDatasetsFilename << std::endl;
    }
    std::cout << "Heartbeat interval:        " << nHeartbeat << std::endl;
    std::cout << "Maximum number of records: " << maxRecords << std::endl;
    std::cout << "Verbosity:                 " << verbosity << std::endl;
  }

}
