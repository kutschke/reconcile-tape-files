#ifndef Record_hh
#define Record_hh

#include <string>

struct Record {
public:
  Record ( std::string const& line );

  bool isExpectedFile() const { return !isAggregation && !isScratch; }

  // Read from file.
  std::string storage_group;
  std::string file_family;
  std::string volume;
  std::string location_cookie;
  std::string bfid;
  long long  size = 0;
  long long  crc  = 0;
  std::string pnfs_id;
  std::string pnfs_path;
  std::string archive_status;
  std::string package_pnfsid;

  // Derived quantities
  std::string name;
  std::string dsname;
  bool isAggregation  = false;
  bool isScratch      = false;
  bool isTDR          = false;
  bool isPackage      = false;
  bool isRegularFile  = false;
};

inline std::ostream& operator<<(std::ostream& ost,
                                const Record& r );

#endif  // Record
