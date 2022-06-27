#ifndef FileFamily_hh
#define FileFamily_hh

#include <string>

struct FileFamily {
public:

  FileFamily (){}

  void addFile( long long size );

  long long nFoundSize  = 0; // Total size of all files, found in the listing, in MB
  long nFoundFiles      = 0; // Number of files found in the listing

};

#endif  // FileFamily
