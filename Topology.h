#ifndef _TOPOLOGY_H
#define _TOPOLOGY_H

#include "Network.h"
#include <vector>

using namespace std;

class Topology {
public:
  // *** Variables ***
  Topology(Network *net) { network = net; }
  ~Topology() {}

  // *** Functions ***
  void
  read_topology();

private:
  Network *network;
};

#endif
