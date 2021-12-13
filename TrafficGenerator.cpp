#include "TrafficGenerator.h"
#include <cmath>

using namespace std;

void
TrafficGenerator::gen_unicast_sd(unsigned int *src, unsigned int *dest) {
  do {
    *src = uniform_rv(network->numNodes);
  }
  while(*src == network->numNodes);

  do {
    *dest = uniform_rv(network->numNodes);
  }
  while((*dest == *src) || (*dest == network->numNodes));
}

void
TrafficGenerator::gen_temporal_parameters(double *time, double LorM) {
  *time = exponential_rv(LorM);
}

void
TrafficGenerator::gen_first_request() {
  unsigned int      src, dest, occupiedSpectralSlots, dataSize;
  double       arrivalTime = 0;
  double       duration;
  double       bookAheadTime = 0;
  shared_ptr<Event> event_ptr;
  Request_t         requestType;

  gen_unicast_sd(&src, &dest);
  gen_temporal_parameters(&duration, network->mu);
  duration = ceil(duration * (1000 / TIMESLOT_LEN)) / (1000 / TIMESLOT_LEN);

  /* Types: 1. 400AR; 2. 400IR; 3, 100AR; 4. 100IR; 5. 40AR; 6. 40IR */
  do {
    auto type = rand() % 6 + 1;
    switch(type) {
      case 1:
        if(network->trafficGen_numRequests_400AR > 0) {
          network->trafficGen_numRequests_400AR--;
          dataSize      = 400;
          requestType = c_AR;
          bookAheadTime = (int)1 / network->mu;
        }
        else
          dataSize = 0;
        break;
      case 2:
        if(network->trafficGen_numRequests_400IR > 0) {
          network->trafficGen_numRequests_400IR--;
          dataSize    = 400;
          requestType = c_IR;
        }
        else
          dataSize = 0;
        break;
      case 3:
        if(network->trafficGen_numRequests_100AR > 0) {
          network->trafficGen_numRequests_100AR--;
          dataSize      = 100;
          requestType   = c_AR;
          bookAheadTime = (int)1 / network->mu;
        }
        else
          dataSize = 0;
        break;
      case 4:
        if(network->trafficGen_numRequests_100IR > 0) {
          network->trafficGen_numRequests_100IR--;
          dataSize    = 100;
          requestType = c_IR;
        }
        else
          dataSize = 0;
        break;
      case 5:
        if(network->trafficGen_numRequests_40AR > 0) {
          network->trafficGen_numRequests_40AR--;
          dataSize      = 40;
          requestType   = c_AR;
          bookAheadTime = (int)1 / network->mu;
        }
        else
          dataSize = 0;
        break;
      case 6:
        if(network->trafficGen_numRequests_40IR > 0) {
          network->trafficGen_numRequests_40IR--;
          dataSize    = 40;
          requestType = c_IR;
        }
        else
          dataSize = 0;
        break;
    }
  }
  while(dataSize == 0);

  occupiedSpectralSlots = ceil((double)dataSize / BW_SPECSLOT);

  event_ptr = make_shared<CircuitRequest>(
      src, dest, arrivalTime, bookAheadTime, duration, dataSize,
      occupiedSpectralSlots, network->requestCounter, requestType);
  network->requestCounter++;

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
  shared_ptr<Event> event_ptr;
  Request_t         requestType;

  gen_unicast_sd(&src, &dest);
  gen_temporal_parameters(&duration, network->mu);
  duration = ceil(duration * (1000 / TIMESLOT_LEN)) / (1000 / TIMESLOT_LEN);
  gen_temporal_parameters(&time, network->lambda);
  time = ceil(time * (1000 / TIMESLOT_LEN)) / (1000 / TIMESLOT_LEN);

  /* Types: 1. 400AR; 2. 400IR; 3, 100AR; 4. 100IR; 5. 40AR; 6. 40IR */
  do {
    auto type = rand() % 6 + 1;
    switch(type) {
      case 1:
        if(network->trafficGen_numRequests_400AR > 0) {
          network->trafficGen_numRequests_400AR--;
          dataSize      = 400;
          requestType   = c_AR;
          bookAheadTime = (int)1 / network->mu;
        }
        else
          dataSize = 0;
        break;
      case 2:
        if(network->trafficGen_numRequests_400IR > 0) {
          network->trafficGen_numRequests_400IR--;
          dataSize    = 400;
          requestType = c_IR;
        }
        else
          dataSize = 0;
        break;
      case 3:
        if(network->trafficGen_numRequests_100AR > 0) {
          network->trafficGen_numRequests_100AR--;
          dataSize      = 100;
          requestType   = c_AR;
          bookAheadTime = (int)1 / network->mu;
        }
        else
          dataSize = 0;
        break;
      case 4:
        if(network->trafficGen_numRequests_100IR > 0) {
          network->trafficGen_numRequests_100IR--;
          dataSize    = 100;
          requestType = c_IR;
        }
        else
          dataSize = 0;
        break;
      case 5:
        if(network->trafficGen_numRequests_40AR > 0) {
          network->trafficGen_numRequests_40AR--;
          dataSize      = 40;
          requestType   = c_AR;
          bookAheadTime = (int)1 / network->mu;
        }
        else
          dataSize = 0;
        break;
      case 6:
        if(network->trafficGen_numRequests_40IR > 0) {
          network->trafficGen_numRequests_40IR--;
          dataSize    = 40;
          requestType = c_IR;
        }
        else
          dataSize = 0;
        break;
    }
  }
  while(dataSize == 0);

  occupiedSpectralSlots = ceil((double)dataSize / BW_SPECSLOT);
  arrivalTime           = systemTime + time;

  event_ptr = make_shared<CircuitRequest>(
      src, dest, arrivalTime, bookAheadTime, duration, dataSize,
      occupiedSpectralSlots, network->requestCounter, requestType);

  network->requestCounter++;

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
