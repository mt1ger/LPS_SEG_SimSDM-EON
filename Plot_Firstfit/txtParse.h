#ifndef _H_TXTPARSE
#define _H_TXTPARSE

#include <cmath>
#include <string>
#include <vector>

using namespace std;

class Metric
{
public:
  Metric(string name) { this->name = name; }
  ~Metric() {}

  string                         name;
  string                         fileName;
  string                         dataString;
  double                         dataNumber;
  vector<vector<vector<double>>> array;
  vector<vector<double>>         array_sec;
  vector<double>                 array_third;
};

class txtParse
{

public:
  txtParse(vector<int> &Cores, vector<int> &Erlangs, vector<int> &Seeds,
           string path)
  {
    potentialCores   = Cores;
    potentialErlangs = Erlangs;
    potentialSeeds   = Seeds;
    this->path       = path;
  }
  ~txtParse() {}

  void
  parse();

  void
  initialization(Metric *metric_ptr);

  void
  extraction(const string &fileContent, Metric *metric_ptr, string &core,
             string &erlang, string &seed);

  void
  avg_data(Metric *metric_ptr);

  void
  write_to_files(Metric *metric_ptr);

private:
  string      iFileName = "Plot";
  string      iFileFullPath;
  string      path;
  vector<int> potentialCores;
  vector<int> potentialErlangs;
  vector<int> potentialSeeds;
};

#endif
