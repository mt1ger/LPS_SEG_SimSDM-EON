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

  if(argc != 9) {
    cout << "Please input arguments in the following order: " << endl;
    cout << "\tThe file for network topology" << endl;
    cout << "\tThe path to store results" << endl;
    cout << "\tTotal number of requests" << endl;
    cout << "\tThe number of cores" << endl;
    cout << "\tAverage arriving rate of request (Lambda)" << endl;
    cout << "\tAverage holding time (1 / Mu)" << endl;
    cout << "\tSeed for random number generation" << endl;
    cout << "\tAllowed number of segments" << endl;
    cout << endl;
    exit(0);
  }

  strcpy(network->fileName, argv[1]);
  path                   = argv[2];
  network->numofRequests = atol(argv[3]);
  network->numofCores    = atoi(argv[4]);
  network->lambda        = atof(argv[5]);
  network->mu            = atof(argv[6]);
  srand(atof(argv[7]));
  network->maxAllowedSegments = atof(argv[8]);

  chrono::high_resolution_clock::time_point start;
  chrono::high_resolution_clock::time_point end;

  start = chrono::high_resolution_clock::now();

  network->init();

  network->simulation();
  double erlang = network->lambda / network->mu;
  double blockingProbability
      = (double)network->numofFailedRequests / (double)network->numofRequests;

  fstream file_ptr;

  // Begin???: Still needed? for Max segments used
  // string  filePath;
  // filePath = path + "MaxSec.txt";
  // file_ptr.open(filePath, fstream::app);
  // file_ptr << to_string(network->maxNumofSegments) + ' ';
  // file_ptr.close();
  // End???

  string resultFile; // file for storing sim results
  resultFile = path + "Plot.txt";
  file_ptr.open(resultFile, fstream::app);
  string plot
      = to_string(network->numofCores) + ',' + to_string(erlang) + ',' + argv[7]
        + ',' + to_string(blockingProbability) + ','
        + to_string(network->maxNumofTransponders) + ','
        + to_string(network->avgCoresUsed) + ','
        + to_string(network->avgHoldingTime) + ','
        + to_string(network->avgTranspondersUsed) + ','
        + to_string(network->avgLPS) + ',' + to_string(network->avgIntFrag)
        + ',' + to_string(network->avgExtFrag) + ','
        + to_string(network->avgHybridFrag) + ','
        + to_string(network->numof400SC6) + ','
        + to_string(network->numof400SC4) + ','
        + to_string(network->numof400SC2) + ','
        + to_string(network->numof200SC6) + ','
        + to_string(network->numof200SC4) + ','
        + to_string(network->numof200SC2) + ','
        + to_string(network->numof100SC6) + ','
        + to_string(network->numof100SC4) + ','
        + to_string(network->numof100SC2) + ',' + to_string(network->numof50SC6)
        + ',' + to_string(network->numof50SC4) + ','
        + to_string(network->numof50SC2) + ',' + to_string(network->numof25SC)
        + ',' + to_string(network->block_400) + ','
        + to_string(network->block_100) + ',' + to_string(network->block_40)
        + '\n';
  file_ptr << plot;
  file_ptr.close();

  cout << "The blocked request distribution: (40, 100, 400)" << endl;
  cout << network->block_40 << ' ' << network->block_100 << ' '
       << network->block_400 << endl;

  end = chrono::high_resolution_clock::now();
  chrono::duration<double> timeSpent
      = chrono::duration_cast<chrono::duration<double>>(end - start);
  cout << "Time spent in \"s\" is " << timeSpent.count() << endl;
  cout << "************************************************************"
       << endl;

  return 1;
}
