#include "FileFamily.hh"

void FileFamily::addFile( long long size ) {
  ++nFoundFiles;
  nFoundSize += size;
}
