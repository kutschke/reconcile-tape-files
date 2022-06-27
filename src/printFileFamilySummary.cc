#include "printFileFamilySummary.hh"

#include <fstream>
#include <iomanip>

using namespace std;

void printFileFamilySummary( FileFamilies const& families, std::string const& outputFileName ){

  long sumFiles{0};
  long long sumSize{0};

  ofstream out( outputFileName );
  for ( auto const& f : families.families ){
    sumFiles += f.second.nFoundFiles;
    sumSize  += f.second.nFoundSize;
    out << setw(12) << f.second.nFoundFiles
        << setw(12) << f.second.nFoundSize/1000000
        << "  "
        << f.first
        << endl;
  }
  out << "----------------------------------" << endl;
  out << setw(12) << sumFiles
      << setw(12) << sumSize/1000000
      << "  Total"
      << endl;
}
