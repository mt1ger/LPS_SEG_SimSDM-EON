// #define DEBUG_test_sequence_in_queue
// #define DEBUG_probe_NumofDoneRequests_and_NumofRequests
#define DEBUG_print_EventID_of_blocked_requests

#include "Network.h"
#include "Event.h"
#include "RoutingTable.h"
#include "TrafficGenerator.h"
#include <iostream>

#include "ResourceAssignment_Firstfit.h"

using namespace std;

void
Network::init() {

  RoutingTable routingTable(this);
  routingTable.generate_routing_table();

  vector<bool>                 spectralSlotIndex;
  vector<vector<bool>>         coreIndex;
  vector<vector<vector<bool>>> successorIndex;

  for(unsigned int i = 0; i < numNodes; i++) {
    for(unsigned int j = 0; j < numNodes; j++) {
      for(unsigned int c = 0; c < numCores; c++) {
        for(int k = 0; k < NUMOFSPECTRALSLOTS; k++) {
          spectralSlotIndex.push_back(false);
        }
        coreIndex.push_back(spectralSlotIndex);
        spectralSlotIndex.clear();
      }
      successorIndex.push_back(coreIndex);
      coreIndex.clear();
    }
    resourceSnapshot.push_back(successorIndex);
    successorIndex.clear();
  }

  firstMiliSec         = 0;
  lastMiliSec          = -1; // the reason for not 0 is for the first request.
  systemClock          = 0;
  requestCounter       = 0;
  numDoneRequests      = 0;
  numAllocations_total = 0;
  numTransponders      = 0;
  numLPSs              = 0;
  numSSs4Data          = 0;
  maxTransponders      = 0;
  maxLPSs              = 0;
  maxSSs4Data          = 0;

  totalHoldingTime = 0;
  totalCoresUsed   = 0;
  numLPSs_total    = 0;
  totalSSs4Data    = 0;
  totalSSsOccupied = 0;
  totalMDataSize   = 0;
  avgExtFrag       = 0;
  avgIntFrag       = 0;
  avgHybridFrag    = 0;

  // Total Blocks
  numBlocks_total = 0;
  numBlocks_AR    = 0;
  numBlocks_IR    = 0;
  numBlocks_400   = 0;
  numBlocks_400AR = 0;
  numBlocks_400IR = 0;
  numBlocks_100   = 0;
  numBlocks_100AR = 0;
  numBlocks_100IR = 0;
  numBlocks_40    = 0;
  numBlocks_40AR  = 0;
  numBlocks_40IR  = 0;

  // Total Transponders
  numTransponders_total = 0;
  numTransponders_AR    = 0;
  numTransponders_IR    = 0;
  numTransponders_400   = 0;
  numTransponders_400AR = 0;
  numTransponders_400IR = 0;
  numTransponders_100   = 0;
  numTransponders_100AR = 0;
  numTransponders_100IR = 0;
  numTransponders_40    = 0;
  numTransponders_40AR  = 0;
  numTransponders_40IR  = 0;

  // Average Transponoders per Request
  avgTransponders       = 0;
  avgTransponders_AR    = 0;
  avgTransponders_IR    = 0;
  avgTransponders_400   = 0;
  avgTransponders_400AR = 0;
  avgTransponders_400IR = 0;
  avgTransponders_100   = 0;
  avgTransponders_100AR = 0;
  avgTransponders_100IR = 0;
  avgTransponders_40    = 0;
  avgTransponders_40AR  = 0;
  avgTransponders_40IR  = 0;

  // Total LPSs
  numLPSs_total = 0;
  numLPSs_AR    = 0;
  numLPSs_IR    = 0;
  numLPSs_400   = 0;
  numLPSs_400AR = 0;
  numLPSs_400IR = 0;
  numLPSs_100   = 0;
  numLPSs_100AR = 0;
  numLPSs_100IR = 0;
  numLPSs_40    = 0;
  numLPSs_40AR  = 0;
  numLPSs_40IR  = 0;

  // Average LPS per Request
  avgLPSs       = 0;
  avgLPSs_AR    = 0;
  avgLPSs_IR    = 0;
  avgLPSs_400   = 0;
  avgLPSs_400AR = 0;
  avgLPSs_400IR = 0;
  avgLPSs_100   = 0;
  avgLPSs_100AR = 0;
  avgLPSs_100IR = 0;
  avgLPSs_40    = 0;
  avgLPSs_40AR  = 0;
  avgLPSs_40IR  = 0;

  // Light-segment Utilization Distribution
  num400SC6 = 0; // number of 64QAM 400Gb/s super
                 // channel used per simulation
  num400SC4 = 0; // number of 16QAM 400Gb/s super
                 // channel used per simulation
  num400SC2 = 0; // number of QPSK 400Gb/s super
                 // channel used per simulation
  num200SC6 = 0; // number of 64QAM 200Gb/s super
                 // channel used per simulation
  num200SC4 = 0; // number of 16QAM 200Gb/s super
                 // channel used per simulation
  num200SC2 = 0; // number of QPSK 200Gb/s super
                 // channel used per simulation
  num100SC6 = 0; // number of 64QAM 100Gb/s super
                 // channel used per simulation
  num100SC4 = 0; // number of 16QAM 100Gb/s super
                 // channel used per simulation
  num100SC2 = 0; // number of QPSK 100Gb/s super
                 // channel used per simulation
  num50SC6 = 0;  // number of 64QAM 50Gb/s super
                 // channel used per simulation
  num50SC4 = 0;  // number of 16QAM 50Gb/s super
                 // channel used per simulation
  num50SC2 = 0;  // number of QPSK 50Gb/s super channel
                 // used per simulation
  num25SC = 0;   // number of 25Gb/s super channel used per simulation

  trafficGen_numRequests_400AR
      = (long long)(numRequests * ratio_400 * AR_RATIO);
  trafficGen_numRequests_400IR
      = (long long)(numRequests * ratio_400) - trafficGen_numRequests_400AR;
  trafficGen_numRequests_100AR
      = (long long)(numRequests * ratio_100 * AR_RATIO);
  trafficGen_numRequests_100IR
      = (long long)(numRequests * ratio_100) - trafficGen_numRequests_100AR;
  trafficGen_numRequests_40AR = (long long)(numRequests * ratio_40 * AR_RATIO);
  trafficGen_numRequests_40IR
      = (long long)(numRequests * ratio_40) - trafficGen_numRequests_40AR;

  /* cout << "Number of Requests Plan Breakdown: (400AR, 400IR, 100AR, 100IR, "
   */
  /*         "40AR, 40IR)" */
  /*      << endl; */
  /* cout << trafficGen_numRequests_400AR << ' ' << trafficGen_numRequests_400IR
   */
  /*      << ' ' << trafficGen_numRequests_100AR << ' ' */
  /*      << trafficGen_numRequests_100IR << ' ' << trafficGen_numRequests_40AR
   */
  /*      << ' ' << trafficGen_numRequests_40IR << ' ' << endl; */

  numRequests_AR    = 0;
  numRequests_IR    = 0;
  numRequests_400   = 0;
  numRequests_400AR = 0;
  numRequests_400IR = 0;
  numRequests_100   = 0;
  numRequests_100AR = 0;
  numRequests_100IR = 0;
  numRequests_40    = 0;
  numRequests_40AR  = 0;
  numRequests_40IR  = 0;

  numAllocations_AR    = 0;
  numAllocations_IR    = 0;
  numAllocations_400   = 0;
  numAllocations_400AR = 0;
  numAllocations_400IR = 0;
  numAllocations_100   = 0;
  numAllocations_100AR = 0;
  numAllocations_100IR = 0;
  numAllocations_40    = 0;
  numAllocations_40AR  = 0;
  numAllocations_40IR  = 0;
}

void
Network::simulation() {
  shared_ptr<EventQueue> eventQueue = make_shared<EventQueue>();
  TrafficGenerator       trafficGenerator(this, eventQueue);
  ResourceAssignment     resourceAssignment(this, eventQueue);

  // Generate the first event
  systemClock = 0;
  trafficGenerator.gen_first_request();

  while(!eventQueue->ev_Queue.empty()) {
#ifdef DEBUG_print_sequence_in_queue
    list<shared_ptr<Event>>::iterator iter;
    cout << "PRINT Event key properties in Queue" << endl;
    for(iter = eventQueue->ev_Queue.begin(); iter != eventQueue->ev_Queue.end();
        iter++) {
      cout << (*iter)->eventID << ' ';
      if((*iter)->eventType == c_Release)
        cout << "Release" << ' ';
      else if((*iter)->eventType == c_Request)
        cout << "Request" << ' ';
      cout << (*iter)->eventTime << '\t';
    }
    cout << endl;
#endif

    shared_ptr<Event> event_ptr;
    event_ptr = eventQueue->ev_Queue.front();

    // Push the Systemtime to the EventTime that is the first in the queue
    if(systemClock <= event_ptr->eventTime) {
      systemClock = event_ptr->eventTime;
    }

    // Time Snapshot Abandoning
    long unsigned int cnt = double_precision(systemClock) - firstMiliSec;
    firstMiliSec += cnt;

    // The following block will cause unexpected termination
    // for (time_iter = SpectralSlots.begin (); cnt != 0; time_iter++)
    // {
    // 		SpectralSlots.pop_front ();
    // 		cnt--;
    // }

    if(cnt >= spectralSlots.size())
      spectralSlots.clear();
    else
      while(cnt != 0) {
        spectralSlots.pop_front();
        cnt--;
      }

    if(event_ptr->eventType == c_Request) {
      shared_ptr<CircuitRequest> cr_ptr
          = static_pointer_cast<CircuitRequest>(event_ptr);
      resourceAssignment.handle_requests(cr_ptr);
      if(requestCounter != numRequests)
        trafficGenerator.gen_request(systemClock);
    }
    else if(event_ptr->eventType == c_Release) {
      shared_ptr<CircuitRelease> cr_ptr
          = static_pointer_cast<CircuitRelease>(event_ptr);
      resourceAssignment.handle_releases(cr_ptr);
    }
    eventQueue->ev_Queue.pop_front(); // This will destroy the poped Event *.

    if(numTransponders > maxTransponders)
      maxTransponders = numTransponders;
    if(numLPSs > maxLPSs)
      maxLPSs = numLPSs;

#ifdef DEBUG_probe_NumofDoneReqeusts_and_NumofRequests
    cout << " " << NumofDoneRequests << " and " << NumofRequests << endl;
#endif

    if((numBlocks_total + numAllocations_total) == numRequests)
      break;
  }

  cout << endl
       << "************************************************************"
       << endl;
#ifdef DEBUG_print_EventID_of_blocked_requests
  cout << "Start to print EventID of blocked reqeusts" << endl;
  for(long unsigned int i = 0; i < blockedRequests.size(); i++) {
    cout << blockedRequests[i] << ' ';
  }
  cout << endl;
#endif

  /*** Collect Measurement Metrics ***/
  avgHoldingTime = totalHoldingTime / numAllocations_total;
  avgCores       = (double)totalCoresUsed / numAllocations_total;
  avgIntFrag = (1 - ((double)totalMDataSize / (totalSSs4Data * BW_SPECSLOT)));
  avgExtFrag     = (1 - (double)totalSSs4Data / totalSSsOccupied);
  avgHybridFrag
      = (1 - (double)totalMDataSize / (totalSSsOccupied * BW_SPECSLOT));

  // Average Metrics Computation
  avgTransponders       = (double)numTransponders_total / numAllocations_total;
  avgTransponders_AR    = (double)numTransponders_AR / numAllocations_AR;
  avgTransponders_IR    = (double)numTransponders_IR / numAllocations_IR;
  avgTransponders_400   = (double)numTransponders_400 / numAllocations_400;
  avgTransponders_400AR = (double)numTransponders_400AR / numAllocations_400AR;
  avgTransponders_400IR = (double)numTransponders_400IR / numAllocations_400IR;
  avgTransponders_100   = (double)numTransponders_100 / numAllocations_100;
  avgTransponders_100AR = (double)numTransponders_100AR / numAllocations_100AR;
  avgTransponders_100IR = (double)numTransponders_100IR / numAllocations_100IR;
  avgTransponders_40    = (double)numTransponders_40 / numAllocations_40;
  avgTransponders_40AR  = (double)numTransponders_40AR / numAllocations_40AR;
  avgTransponders_40IR  = (double)numTransponders_40IR / numAllocations_40IR;

  avgLPSs       = (double)numLPSs_total / numAllocations_total;
  avgLPSs_AR    = (double)numLPSs_AR / numAllocations_AR;
  avgLPSs_IR    = (double)numLPSs_IR / numAllocations_IR;
  avgLPSs_400   = (double)numLPSs_400 / numAllocations_400;
  avgLPSs_400AR = (double)numLPSs_400AR / numAllocations_400AR;
  avgLPSs_400IR = (double)numLPSs_400IR / numAllocations_400IR;
  avgLPSs_100   = (double)numLPSs_100 / numAllocations_100;
  avgLPSs_100AR = (double)numLPSs_100AR / numAllocations_100AR;
  avgLPSs_100IR = (double)numLPSs_100IR / numAllocations_100IR;
  avgLPSs_40    = (double)numLPSs_40 / numAllocations_40;
  avgLPSs_40AR  = (double)numLPSs_40AR / numAllocations_40AR;
  avgLPSs_40IR  = (double)numLPSs_40IR / numAllocations_40IR;

  // cout << "Spectrum Utilization is: " << spectrumUtilization << endl;
  cout << "Requests Breakdown: (AR, IR, 400, 400AR, 400IR, 100, "
          "100AR, 100IR, 40, 40AR, 40IR)"
       << endl;
  cout << numRequests_AR << ' ' << numRequests_IR << ' ' << numRequests_400
       << ' ' << numRequests_400AR << ' ' << numRequests_400IR << ' '
       << numRequests_100 << ' ' << numRequests_100AR << ' '
       << numRequests_100IR << ' ' << numRequests_40 << ' ' << numRequests_40AR
       << ' ' << numRequests_40IR << endl;
  cout << "Allocationed Requests Breakdown: (AR, IR, 400, 400AR, 400IR, 100, "
          "100AR, 100IR, 40, 40AR, 40IR)"
       << endl;
  cout << numAllocations_AR << ' ' << numAllocations_IR << ' '
       << numAllocations_400 << ' ' << numAllocations_400AR << ' '
       << numAllocations_400IR << ' ' << numAllocations_100 << ' '
       << numAllocations_100AR << ' ' << numAllocations_100IR << ' '
       << numAllocations_40 << ' ' << numAllocations_40AR << ' '
       << numAllocations_40IR << endl;
  cout << "Blocked Requests Breakdown: (AR, IR, 400, 400AR, 400IR, 100, 100AR, "
          "100IR, 40, 40AR, 40IR)"
       << endl;
  cout << numBlocks_AR << ' ' << numBlocks_IR << ' ' << numBlocks_400 << ' '
       << numBlocks_400AR << ' ' << numBlocks_400IR << ' ' << numBlocks_100
       << ' ' << numBlocks_100AR << ' ' << numBlocks_100IR << ' '
       << numBlocks_40 << ' ' << numBlocks_40AR << ' ' << numBlocks_40IR
       << endl;
  cout << "Number of Transponders Used Breakdown: (AR, IR, 400, 400AR, 400IR, "
          "100, 100AR, 100IR, 40, 40AR, 40IR)"
       << endl;
  cout << numTransponders_AR << ' ' << numTransponders_IR << ' '
       << numTransponders_400 << ' ' << numTransponders_400AR << ' '
       << numTransponders_400IR << ' ' << numTransponders_100 << ' '
       << numTransponders_100AR << ' ' << numTransponders_100IR << ' '
       << numTransponders_40 << ' ' << numTransponders_40AR << ' '
       << numTransponders_40IR << endl;
  cout << "Average Utilization of Transponders Breakdown: (AR, IR, 400, 400AR, "
          "400IR, 100, 100AR, 100IR, 40, 40AR, 40IR)"
       << endl;
  cout << avgTransponders_AR << ' ' << avgTransponders_IR << ' '
       << avgTransponders_400 << ' ' << avgTransponders_400AR << ' '
       << avgTransponders_400IR << ' ' << avgTransponders_100 << ' '
       << avgTransponders_100AR << ' ' << avgTransponders_100IR << ' '
       << avgTransponders_40 << ' ' << avgTransponders_40AR << ' '
       << avgTransponders_40IR << endl;
  cout << "Utilization of LPSs breakdown: (AR, IR, 400, 400AR, 400IR, 100, "
          "100AR, 100IR, 40, 40AR, 40IR)"
       << endl;
  cout << numLPSs_AR << ' ' << numLPSs_IR << ' ' << numLPSs_400 << ' '
       << numLPSs_400AR << ' ' << numLPSs_400IR << ' ' << numLPSs_100 << ' '
       << numLPSs_100AR << ' ' << numLPSs_100IR << ' ' << numLPSs_40 << ' '
       << numLPSs_40AR << ' ' << numLPSs_40IR << endl;
  cout << "Average Utilization of LPSs breakdown: (AR, IR, 400, 400AR, 400IR, "
          "100, 100AR, 100IR, 40, 40AR, 40IR)"
       << endl;
  cout << avgLPSs_AR << ' ' << avgLPSs_IR << ' ' << avgLPSs_400 << ' '
       << avgLPSs_400AR << ' ' << avgLPSs_400IR << ' ' << avgLPSs_100 << ' '
       << avgLPSs_100AR << ' ' << avgLPSs_100IR << ' ' << avgLPSs_40 << ' '
       << avgLPSs_40AR << ' ' << avgLPSs_40IR << endl;
  /* cout << "Utilization of different SCs (400: 64QAM, 400: 16QAM, 400: QPSK, "
   */ // This does not work due to same request may use different LS type in different time slot!
  /*         "200: 64QAM, 200: 16QAM, 200: QPSK, 100: 64QAM, 100: 16QAM, 100: "
   */
  /*         "QPSK, 50: 64QAM, 50: 16QAM, 50: QPSK, 25): " */
  /*      << num400SC6 << ' ' << num400SC4 << ' ' << num400SC2 << ' ' */
  /*      << num200SC6 << ' ' << num200SC4 << ' ' << num200SC2 << ' ' */
  /*      << num100SC6 << ' ' << num100SC4 << ' ' << num100SC2 << ' ' */
  /*      << num50SC6 << ' ' << num50SC4 << ' ' << num50SC2 << ' ' */
  /*      << num25SC << endl; */
  cout << "# of blocked requests is " << numBlocks_total << endl;
  cout << "Blocking Probability: "
       << (double)numBlocks_total / (double)numRequests << endl;
  cout << "Max # of Transponders used: " << maxTransponders << endl;
  cout << "Max # of LPS used: " << maxLPSs << endl;
  cout << "Average Transponders Used per Request: " << avgTransponders << endl;
  cout << "Average LPS per Request: " << avgLPSs << endl;
  cout << "Network Load: " << lambda / mu << " Erlang" << endl;
  cout << "Average Cores Used Used per Request: " << avgCores << endl;
  cout << "Average Holding Time per Request: " << avgHoldingTime << endl;
  cout << "Average Internal Fragmentation: " << avgIntFrag << endl;
  cout << "Average External Fragmentation: " << avgExtFrag << endl;
  cout << "Average Hybrid Fragmentation: " << avgHybridFrag << endl;
}
