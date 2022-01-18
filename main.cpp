#include <chrono>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <vector>

#include "Network.h"
#include "RoutingTable.h"

using namespace std;



int
main(int argc, char *argv[]) {
  string   path;
  Network *network;
  Network  net;

  network = &net;

  if(argc != 10) {
    cout << "Please input arguments in the following order: " << endl;
    cout << "\tThe file for network topology" << endl;
    cout << "\tThe path to store results" << endl;
    cout << "\tTotal number of requests" << endl;
    cout << "\tThe number of cores" << endl;
    cout << "\tAverage arriving rate of request (Lambda)" << endl;
    cout << "\tAverage holding time (1 / Mu)" << endl;
    cout << "\tMaximum number of light-segments for each request" << endl;
    cout << "\tMaximum number of time slices for each request" << endl;
    cout << "\tSeed for random number generation" << endl;
    cout << endl;
    exit(0);
  }

  strcpy(network->fileName, argv[1]);
  path                   = argv[2];
  network->numRequests = atol(argv[3]);
  network->numCores    = atoi(argv[4]);
  network->lambda        = atof(argv[5]);
  network->mu            = atof(argv[6]);
  network->maxAllowedLightSegments = atof(argv[7]);
  network->maxAllowedTimeSlices    = atof(argv[8]);
  network->seed                    = atof(argv[9]);
  srand(network->seed);

  cout << "************************************************************"
       << endl;
  cout << "The file for network topology: " << network->fileName << endl;
  cout << "The path to store results: " << path << endl;
  cout << "Total number of requests: " << network->numRequests << endl;
  cout << "The number of cores: " << network->numCores << endl;
  cout << "Average arriving rate of request (Lambda): " << network->lambda
       << endl;
  cout << "Average holding time (1 / Mu): " << network->mu << endl;
  cout << "Maximum number of light-segments for each request: "
       << network->maxAllowedLightSegments << endl;
  cout << "Maximum number of time slices for each request: "
       << network->maxAllowedTimeSlices << endl;
  cout << "Seed for random number generation: " << network->seed << endl;
  cout << "************************************************************"
       << endl;

  chrono::high_resolution_clock::time_point start;
  chrono::high_resolution_clock::time_point end;
  start = chrono::high_resolution_clock::now();

  network->init();

  network->simulation();
  double erlang = network->lambda / network->mu;
  double blockingProbability
      = (double)network->numBlocks_total / (double)network->numRequests;

  fstream file_ptr;

  // ??? Begin: Still needed? for Max segments used
  // string  filePath;
  // filePath = path + "MaxSec.txt";
  // file_ptr.open(filePath, fstream::app);
  // file_ptr << to_string(network->maxLightSegments) + ' ';
  // file_ptr.close();
  // ??? End

  string resultFile; // file for storing sim results
  resultFile = path + "Plot.txt";
  file_ptr.open(resultFile, fstream::app);
  string plot
      = to_string(network->numCores) + ',' + to_string(erlang) + ','
        + to_string(network->seed) + ',' + to_string(blockingProbability) + ','
        + to_string(network->maxTransponders) + ','
        + to_string(network->numTransponders_total) + ','
        + to_string(network->maxLPSs) + ',' + to_string(network->avgCores) + ','
        + to_string(network->avgHoldingTime) + ','
        + to_string(network->avgTransponders) + ','
        + to_string(network->avgLPSs) + ',' + to_string(network->avgIntFrag)
        + ',' + to_string(network->avgExtFrag) + ','
        + to_string(network->avgHybridFrag) + ','
        + to_string(network->num400SC6) + ',' + to_string(network->num400SC4)
        + ',' + to_string(network->num400SC2) + ','
        + to_string(network->num200SC6) + ',' + to_string(network->num200SC4)
        + ',' + to_string(network->num200SC2) + ','
        + to_string(network->num100SC6) + ',' + to_string(network->num100SC4)
        + ',' + to_string(network->num100SC2) + ','
        + to_string(network->num50SC6) + ',' + to_string(network->num50SC4)
        + ',' + to_string(network->num50SC2) + ',' + to_string(network->num25SC)
        + ',' + to_string(network->numBlocks_AR) + ','
        + to_string(network->numBlocks_IR) + ','
        + to_string(network->numBlocks_400) + ','
        + to_string(network->numBlocks_400AR) + ','
        + to_string(network->numBlocks_400IR) + ','
        + to_string(network->numBlocks_100) + ','
        + to_string(network->numBlocks_100AR) + ','
        + to_string(network->numBlocks_100IR) + ','
        + to_string(network->numBlocks_40) + ','
        + to_string(network->numBlocks_40AR) + ','
        + to_string(network->numBlocks_40IR) + ','
        + to_string(network->numTransponders_AR) + ','
        + to_string(network->numTransponders_IR) + ','
        + to_string(network->numTransponders_400) + ','
        + to_string(network->numTransponders_400AR) + ','
        + to_string(network->numTransponders_400IR) + ','
        + to_string(network->numTransponders_100) + ','
        + to_string(network->numTransponders_100AR) + ','
        + to_string(network->numTransponders_100IR) + ','
        + to_string(network->numTransponders_40) + ','
        + to_string(network->numTransponders_40AR) + ','
        + to_string(network->numTransponders_40IR) + ','
        + to_string(network->avgTransponders_AR) + ','
        + to_string(network->avgTransponders_IR) + ','
        + to_string(network->avgTransponders_400) + ','
        + to_string(network->avgTransponders_400AR) + ','
        + to_string(network->avgTransponders_400IR) + ','
        + to_string(network->avgTransponders_100) + ','
        + to_string(network->avgTransponders_100AR) + ','
        + to_string(network->avgTransponders_100IR) + ','
        + to_string(network->avgTransponders_40) + ','
        + to_string(network->avgTransponders_40AR) + ','
        + to_string(network->avgTransponders_40IR) + ','
        + to_string(network->numLPSs_AR) + ',' + to_string(network->numLPSs_IR)
        + ',' + to_string(network->numLPSs_400) + ','
        + to_string(network->numLPSs_400AR) + ','
        + to_string(network->numLPSs_400IR) + ','
        + to_string(network->numLPSs_100) + ','
        + to_string(network->numLPSs_100AR) + ','
        + to_string(network->numLPSs_100IR) + ','
        + to_string(network->numLPSs_40) + ','
        + to_string(network->numLPSs_40AR) + ','
        + to_string(network->numLPSs_40IR) + ','
        + to_string(network->avgLPSs_AR) + ',' + to_string(network->avgLPSs_IR)
        + ',' + to_string(network->avgLPSs_400) + ','
        + to_string(network->avgLPSs_400AR) + ','
        + to_string(network->avgLPSs_400IR) + ','
        + to_string(network->avgLPSs_100) + ','
        + to_string(network->avgLPSs_100AR) + ','
        + to_string(network->avgLPSs_100IR) + ','
        + to_string(network->avgLPSs_40) + ','
        + to_string(network->avgLPSs_40AR) + ','
        + to_string(network->avgLPSs_40IR) + '\n';
  file_ptr << plot;
  file_ptr.close();

  end = chrono::high_resolution_clock::now();
  chrono::duration<double> timeSpent
      = chrono::duration_cast<chrono::duration<double>>(end - start);
  cout << "Time spent in \"s\" is " << timeSpent.count() << endl;
  cout << "************************************************************"
       << endl;

  return 1;
}
