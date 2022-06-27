#include "Record.hh"

#include <iostream>
#include <sstream>

using namespace std;

Record::Record ( string const& line ){
  istringstream in(line.c_str());
  in >> storage_group;
  in >> file_family;
  in >> volume;
  in >> location_cookie;
  in >> bfid;
  if ( !in ) cout << "Error ... " << endl;
  in >> size;
  in >> crc;
  in >> pnfs_id;
  in >> pnfs_path;
  in >> archive_status;
  in >> package_pnfsid;

  isAggregation = line.find("/file_aggregation/") != string::npos;
  if ( ! isAggregation ) {
    isScratch     = line.find("/scratch/") != string::npos;
    auto i = pnfs_path.rfind('/');
    if ( i != string::npos ){
      name = pnfs_path.substr(i+1);
    }
    if ( !isScratch ){
      auto j = name.rfind('.');
      if  ( j != string::npos ){
        string ext = name.substr(j);
        auto k = name.substr(0,j).rfind('.');
        if ( k != string::npos ){
          dsname = name.substr(0,k)+ext;
        }
      }
    }
  }
}

inline std::ostream& operator<<(std::ostream& ost,
                                const Record& r ){
  ost << r.storage_group << " "
      << r.file_family << " "
      << r.volume << " "
      << r.location_cookie << " "
      << r.bfid << " "
      << r.size << " "
      << r.crc << " "
      << r.pnfs_id << " "
      << r.pnfs_path << " "
      << r.archive_status << " "
      << r.package_pnfsid << "  | "
      << r.isScratch;
  return ost;
}
