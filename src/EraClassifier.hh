#ifndef EraClassifier_hh
#define EraClassifier_hh

//
// Classify a file as belonging to one or several eras.
//  CD3, MDC2018, MDC2020 etc
//


class Record;

#include <map>
#include <string>

class EraClassifier {

public:

  struct Stats{
    Stats(){}
    long nFiles = 0;
    long long size = 0;
    void addFile ( long long asize);
  };

  EraClassifier(){};

  void addFile ( Record const& r );

  std::map< std::string, Stats> const& stats() const { return _stats;}

private:

  std::map< std::string, Stats> _stats;

};

#endif
