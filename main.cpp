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
    cout << "\tSeed for random number generation" << endl;
    cout << "\tMaximum number of light-segments for each request" << endl;
    cout << "\tMaximum number of time slices for each request" << endl;
    cout << endl;
    exit(0);
  }

  strcpy(network->fileName, argv[1]);
  path                   = argv[2];
  network->numRequest = atol(argv[3]);
  network->numCores    = atoi(argv[4]);
  network->lambda        = atof(argv[5]);
  network->mu            = atof(argv[6]);
  srand(atof(argv[7]));
  network->maxLightSegments = atof(argv[8]);
  network->maxTimeSlices    = atof(argv[9]);

  chrono::high_resolution_clock::time_point start;
  chrono::high_resolution_clock::time_point end;

  start = chrono::high_resolution_clock::now();

  network->init();

  network->simulation();
  double erlang = network->lambda / network->mu;
  double blockingProbability
      = (double)network->totalBlocks / (double)network->numRequest;

  fstream file_ptr;

  // ??? Begin: Still needed? for Max segments used
  // string  filePath;
  // filePath = path + "MaxSec.txt";
  // file_ptr.open(filePath, fstream::app);
  // file_ptr << to_string(network->maxSegments) + ' ';
  // file_ptr.close();
  // ??? End

  string resultFile; // file for storing sim results
  resultFile = path + "Plot.txt";
  file_ptr.open(resultFile, fstream::app);
  string plot
      = to_string(network->numCores) + ',' + to_string(erlang) + ',' + argv[7]
        + ',' + to_string(blockingProbability) + ','
        + to_string(network->maxTransponders) + ','
        + to_string(network->totalTransponders) + ','
        + to_string(network->avgCores) + ','
        + to_string(network->avgHoldingTime) + ',' + to_string(network->avgTransponders)
        + ',' + to_string(network->LPSspR) + ','
        + to_string(network->avgIntFrag) + ',' + to_string(network->avgExtFrag)
        + ',' + to_string(network->avgHybridFrag) + ','
        + to_string(network->num400SC6) + ','
        + to_string(network->num400SC4) + ','
        + to_string(network->num400SC2) + ','
        + to_string(network->num200SC6) + ','
        + to_string(network->num200SC4) + ','
        + to_string(network->num200SC2) + ','
        + to_string(network->num100SC6) + ','
        + to_string(network->num100SC4) + ','
        + to_string(network->num100SC2) + ',' + to_string(network->num50SC6)
        + ',' + to_string(network->num50SC4) + ','
        + to_string(network->num50SC2) + ',' + to_string(network->num25SC)
        + ',' + to_string(network->totalBlocks_AR) + ','
        + to_string(network->totalBlocks_IR) + ','
        + to_string(network->totalBlocks_400) + ','
        + to_string(network->totalBlocks_400AR) + ','
        + to_string(network->totalBlocks_400IR) + ','
        + to_string(network->totalBlocks_100) + ','
        + to_string(network->totalBlocks_100AR) + ','
        + to_string(network->totalBlocks_100IR) + ','
        + to_string(network->totalBlocks_40) + ','
        + to_string(network->totalBlocks_40AR) + ','
        + to_string(network->totalBlocks_40IR) + ','
        + to_string(network->totalTransponders_AR) + ','
        + to_string(network->totalTransponders_IR) + ','
        + to_string(network->totalTransponders_400) + ','
        + to_string(network->totalTransponders_400AR) + ','
        + to_string(network->totalTransponders_400IR) + ','
        + to_string(network->totalTransponders_100) + ','
        + to_string(network->totalTransponders_100AR) + ','
        + to_string(network->totalTransponders_100IR) + ','
        + to_string(network->totalTransponders_40) + ','
        + to_string(network->totalTransponders_40AR) + ','
        + to_string(network->totalTransponders_40IR) + ','
        + to_string(network->avgTransponders_AR) + ',' + to_string(network->avgTransponders_IR) + ','
        + to_string(network->avgTransponders_400) + ',' + to_string(network->avgTransponders_400AR)
        + ',' + to_string(network->avgTransponders_400IR) + ','
        + to_string(network->avgTransponders_100) + ',' + to_string(network->avgTransponders_100AR)
        + ',' + to_string(network->avgTransponders_100IR) + ',' + to_string(network->avgTransponders_40)
        + ',' + to_string(network->avgTransponders_40AR) + ','
        + to_string(network->avgTransponders_40IR) + ',' + to_string(network->totalLPSs_AR)
        + ',' + to_string(network->totalLPSs_IR) + ','
        + to_string(network->totalLPSs_400) + ','
        + to_string(network->totalLPSs_400AR) + ','
        + to_string(network->totalLPSs_400IR) + ','
        + to_string(network->totalLPSs_100) + ','
        + to_string(network->totalLPSs_100AR) + ','
        + to_string(network->totalLPSs_100IR) + ','
        + to_string(network->totalLPSs_40) + ','
        + to_string(network->totalLPSs_40AR) + ','
        + to_string(network->totalLPSs_40IR) + '\n';
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
