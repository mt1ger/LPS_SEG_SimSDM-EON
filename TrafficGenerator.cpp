#include "TrafficGenerator.h"
#include <cmath>

using namespace std;

void
TrafficGenerator::gen_unicast_sd(unsigned int *src, unsigned int *dest) {
  do {
    *src = uniform_rv(network->numofNodes);
  }
  while(*src == network->numofNodes);

  do {
    *dest = uniform_rv(network->numofNodes);
  }
  while((*dest == *src) || (*dest == network->numofNodes));
}

void
TrafficGenerator::gen_temporal_parameters(double *time, double LorM) {
  *time = exponential_rv(LorM);
}

void
TrafficGenerator::gen_first_request() {
  unsigned int src, dest, occupiedSpectralSlots, datasize;
  double       arrivalTime = 0;
  double       duration;
  double       bookAheadTime = 0;
  int          type          = 0;
  shared_ptr<Event> event_ptr;
  Request_t         requestType;

  gen_unicast_sd(&src, &dest);
  gen_temporal_parameters(&duration, network->mu);
  duration = ceil(duration * 1000) / 1000;

  do {
    do {
      type = uniform_rv(4);
    }
    while(type == 0 || type == 4);
    switch(type) {
      case 1:
        if(network->trafficGen_numofRequests_40 > 0) {
          network->trafficGen_numofRequests_40--;
          datasize = 40;
        }
        else
          datasize = 0;
        break;
      case 2:
        if(network->trafficGen_numofRequests_100 > 0) {
          network->trafficGen_numofRequests_100--;
          datasize = 100;
        }
        else
          datasize = 0;
        break;
      case 3:
        if(network->trafficGen_numofRequests_400 > 0) {
          network->trafficGen_numofRequests_400--;
          datasize = 400;
        }
        else
          datasize = 0;
        break;
    }
  }
  while(datasize == 0);

  occupiedSpectralSlots = ceil((double)datasize / BW_SPECSLOT);

  while(1) {
    int requestTypeNum;
    do {
      requestTypeNum = uniform_rv(2);
    }
    while(requestTypeNum == 2);
    // 0 represents IR and 1 represents AR
    if(requestTypeNum == 1 && network->trafficGen_numofRequests_AR != 0) {
      network->trafficGen_numofRequests_AR--;
      requestType   = c_AR;
      bookAheadTime = (int)1 / network->mu;
      break;
    }
    else if(requestTypeNum == 0 && network->trafficGen_numofRequests_IR != 0) {
      network->trafficGen_numofRequests_IR--;
      requestType = c_IR;
      break;
    }
  }

  event_ptr = make_shared<CircuitRequest>(
      src, dest, arrivalTime, bookAheadTime, duration, datasize,
      occupiedSpectralSlots, network->requestCounter, requestType);
  network->requestCounter++;

  cout << network->trafficGen_numofRequests_AR << ' '
       << network->trafficGen_numofRequests_IR << ' '
       << network->trafficGen_numofRequests_400 << ' '
       << network->trafficGen_numofRequests_100 << ' '
       << network->trafficGen_numofRequests_40 << endl;
#ifdef DEBUG_print_new_built_event
  cout << "\tNew Built event:" << endl;
  shared_ptr<CircuitRequest> request_ptr
      = static_pointer_cast<CircuitRequest>(event_ptr);
  cout << '\t' << "Event ID: " << request_ptr->eventID << ' '
       << "SD Pair: " << request_ptr->src << "-->" << request_ptr->dest
       << " ArrT: " << request_ptr->arrivalTime
       << " BAT: " << request_ptr->bookAheadTime
       << " StartT: " << request_ptr->startTime
       << " Dur: " << request_ptr->duration
       << " RelT: " << request_ptr->releaseTime
       << " DataSize: " << request_ptr->bitRate
       << " OccSS: " << request_ptr->occupiedSpectralSlots << endl;
#endif

  eventQueue->ev_Queue.push_back(event_ptr);
}

// releases' generation will be taken care of in "handle_requests" in
// "ResourceAssignment.cpp"
void
TrafficGenerator::gen_request(double systemTime) {
  unsigned int src, dest, dataSize, occupiedSpectralSlots, bookAheadTime = 0;
  double       time, arrivalTime, duration;
  int          type = 0;
  // CircuitRequest *request;
  shared_ptr<Event> event_ptr;
  Request_t         requestType;

  gen_unicast_sd(&src, &dest);
  gen_temporal_parameters(&duration, network->mu);
  duration = ceil(duration * 1000) / 1000;
  gen_temporal_parameters(&time, network->lambda);
  time = ceil(time * 1000) / 1000;

  // do {
  // 	datasize = uniform_rv (MAX_DATASIZE_REQUEST);
  // } while (datasize == 0);

  do {
    do {
      type = uniform_rv(4);
    }
    while(type == 0 || type == 4);
    switch(type) {
      case 1:
        if(network->trafficGen_numofRequests_40 > 0) {
          network->trafficGen_numofRequests_40--;
          dataSize = 40;
        }
        else
          dataSize = 0;
        break;
      case 2:
        if(network->trafficGen_numofRequests_100 > 0) {
          network->trafficGen_numofRequests_100--;
          dataSize = 100;
        }
        else
          dataSize = 0;
        break;
      case 3:
        if(network->trafficGen_numofRequests_400 > 0) {
          network->trafficGen_numofRequests_400--;
          dataSize = 400;
        }
        else
          dataSize = 0;
        break;
    }
  }
  while(dataSize == 0);

  occupiedSpectralSlots = ceil((double)dataSize / BW_SPECSLOT);
  arrivalTime           = systemTime + time;

  while(1) {
    int requestTypeNum;
    do {
      requestTypeNum = uniform_rv(2);
    }
    while(requestTypeNum == 2);
    // 0 represents IR and 1 represents AR
    if(requestTypeNum == 1 && network->trafficGen_numofRequests_AR != 0) {
      network->trafficGen_numofRequests_AR--;
      requestType   = c_AR;
      bookAheadTime = (int)1 / network->mu;
      break;
    }
    else if(requestTypeNum == 0 && network->trafficGen_numofRequests_IR != 0) {
      network->trafficGen_numofRequests_IR--;
      requestType = c_IR;
      break;
    }
  }

  event_ptr = make_shared<CircuitRequest>(
      src, dest, arrivalTime, bookAheadTime, duration, dataSize,
      occupiedSpectralSlots, network->requestCounter, requestType);
  // request = new CircuitRequest(src, dest, arrivalTime,
  // bookAheadTime, duration,
  //                              datasize, occupiedSpectralSlots,
  // network->requestCounter, requestType);

  network->requestCounter++;

  cout << network->trafficGen_numofRequests_AR << ' '
       << network->trafficGen_numofRequests_IR << ' '
       << network->trafficGen_numofRequests_400 << ' '
       << network->trafficGen_numofRequests_100 << ' '
       << network->trafficGen_numofRequests_40 << endl;
#ifdef DEBUG_print_new_built_event
  cout << "\tNew Built event:" << endl;
  shared_ptr<CircuitRequest> request_ptr
      = static_pointer_cast<CircuitRequest>(event_ptr);
  cout << '\t' << "Event ID: " << request_ptr->eventID << ' '
       << "SD Pair: " << request_ptr->src << "-->" << request_ptr->dest
       << " ArrT: " << request_ptr->arrivalTime
       << " BAT: " << request_ptr->bookAheadTime
       << " StartT: " << request_ptr->startTime
       << " Dur: " << request_ptr->duration
       << " RelT: " << request_ptr->releaseTime
       << " DataSize: " << request_ptr->bitRate
       << " OccSS: " << request_ptr->occupiedSpectralSlots << endl;
#endif

  eventQueue->queue_insert(event_ptr);
}
