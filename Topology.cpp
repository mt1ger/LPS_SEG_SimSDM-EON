#include <iostream>
#include <cstdio>
#include <vector>
#include "Topology.h"

using namespace std;

void
Topology::read_topology(void) {
  FILE *         netTopo;
  int            temp;
  vector<double> nodesWeight_sec;

  netTopo = fopen(network->fileName, "r");

  fscanf(netTopo, "%d", &network->numofNodes);

  for(unsigned int i = 0; i < network->numofNodes; i++) {
    for(unsigned int j = 0; j < network->numofNodes; j++) {
      fscanf(netTopo, "%d", &temp);
      nodesWeight_sec.push_back(temp);
    }

    network->nodesWeight.push_back(nodesWeight_sec);
    nodesWeight_sec.clear();
  }

  fclose(netTopo);
}
