#include "readDeletedDatasets.hh"

#include <fstream>

using namespace std;

vector<string> readDeletedDatasets ( string const& inputFile ){
  vector<string> ds;

  if ( inputFile.empty() ) return ds;

  ifstream in(inputFile);
  while (in){
    string dsname;
    getline(in,dsname);
    if ( !in ){
      break;
    }
    ds.push_back(dsname);
  }

  return ds;
}
