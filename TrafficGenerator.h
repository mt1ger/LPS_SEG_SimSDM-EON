#ifndef _TRAFFICGENERATOR_H
#define _TRAFFICGENERATOR_H

#include "EventQueue.h"
#include "Network.h"
#include "RandomVariable.h"

class TrafficGenerator {
public:
  TrafficGenerator(Network *net, shared_ptr<EventQueue> eq) {
    network    = net;
    eventQueue = eq;
  }
  ~TrafficGenerator() {}

  void
  gen_unicast_sd(unsigned int *src, unsigned int *dest);
  void
  gen_temporal_parameters(double *time, double LorM);
  void
  gen_first_request();
  void
  gen_request(double systemTime);

private:
  Network               *network;
  shared_ptr<EventQueue> eventQueue;
};
#endif
