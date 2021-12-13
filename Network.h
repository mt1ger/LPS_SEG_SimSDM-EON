#ifndef _NETWORK_H
#define _NETWORK_H

#include "EventQueue.h"
#include <map>
#include <memory>
#include <thread>
#include <vector>

#define TESTING
#define DEBUG_collect_eventID_of_blocked_requests

#ifdef TESTING
/** Not frequently needed **/
/* #define DEBUG_print_sequence_in_queue */
/* #define DEBUG_print_available_spectral_slots */

/** Commonly needed **/
#define DEBUG_print_new_built_event
#define DEBUG_print_potential_voids
/* Super Channel info for a specific request */
#define DEBUG_print_SC_info
/* Sorted Super Channel based on their performance */
#define DEBUG_print_sorted_SC
#define DEBUG_collect_eventID_of_blocked_requests
#define PRINT_allocation_block_release
#endif

#define GB                 1 // Guardband
#define NUMOFSPECTRALSLOTS 128
// #define NUMOFSPECTRALSLOTS 40
#define BW_SPECSLOT         12.5
#define AR_RATIO            0.5
#define TIMESLOT_LEN        1

using namespace std;

class RoutingTable;

class Network {
public:
  Network() {}
  ~Network() {}

public:
  /*** Functions ***/
  void
  init();
  void
  simulation();

  /*** Input Variables ***/
  /** Input Variables **/
  char         fileName[500];
  // lambda: the mean of input rate; mu: 1/mu is the mean of holding time
  double       lambda, mu;
  long long    numRequests;
  unsigned int numCores;
  size_t       maxAllowedLightSegments;
  size_t       maxAllowedTimeSlices;

  /* Topology Variables */
  unsigned int                numNodes;
  vector<vector<double>>      nodesWeight;
  vector<vector<vector<int>>> routingTable;

  /* Global Tracker */
  long long requestCounter; // Generated Number of Requests
  double    systemClock;
  long long firstMiliSec; // The first mili-second for spectralSlots
  long long lastMiliSec;  // The last mili-second for spectralSlots

  // Ratio of requests in different types
  double ratio_40  = 0.3;
  double ratio_100 = 0.5;
  double ratio_400 = 0.2;

  /**** Resource Variables ****/
  // The resource snapshot for a time slot
  // [predecessor][successor][core][spectrum slot]
  vector<vector<vector<vector<bool>>>> resourceSnapshot;
  // The resource snapshots of all considered time slot
  // [time slot][predecessor][successor][core][spectrum slot]
  list<vector<vector<vector<vector<bool>>>>> spectralSlots;

  /**** Metrics Collection ****/
  /*** Overall ***/
  // Number of requests which are successfully allocated, released, or blocked
  // will be considered as DoneRequests
  long long numDoneRequests;
  // Collect the number of transponders simultaneously used after
  // at the time slot for a request allocation
  long long numTransponders;
  // Store the maximum number of transponders used simultaneously at any
  // time slot for one simlulation run
  long long maxTransponders;
  long long numLPSs;
  long long maxLPSs;
  // Total number of data spectral slots occupied after a request allocation
  long long numSSs4Data;
  // Maximum number of data spectral slot occupied simultaneously
  long long maxSSs4Data;

  double    totalHoldingTime;
  long long totalCoresUsed;
  double    avgHoldingTime;
  double    avgCores;

  // Metrics to Measure Fregmentation
  double    spectrumUtilization;
  double    totalMDataSize; // The total modulated data size.
  long long totalSSs4Data;
  long long totalSSsOccupied;
  double    avgIntFrag;
  double    avgExtFrag;
  double    avgHybridFrag;

  /*** Break Down Data ***/
  /* Light-segments Utilization Distribution */
  // number of 64QAM 400Gb/s super channel used per simulation
  long long num400SC6;
  // number of 16QAM 400Gb/s super channel used per simulation
  long long num400SC4;
  // number of QPSK 400Gb/s super channel used per simulation
  long long num400SC2;
  long long num200SC6; // number of 64QAM 200Gb/s super
                         // channel used per simulation
  long long num200SC4; // number of 16QAM 200Gb/s super
                         // channel used per simulation
  long long num200SC2; // number of QPSK 200Gb/s super
                         // channel used per simulation
  long long num100SC6; // number of 64QAM 100Gb/s super
                         // channel used per simulation
  long long num100SC4; // number of 16QAM 100Gb/s super
                         // channel used per simulation
  long long num100SC2; // number of QPSK 100Gb/s super
                         // channel used per simulation
  long long num50SC6; // number of 64QAM 50Gb/s super
                      // channel used per simulation
  long long num50SC4; // number of 16QAM 50Gb/s super
                      // channel used per simulation
  long long num50SC2; // number of QPSK 50Gb/s super channel
                      // used per simulation
  long long num25SC;  // number of 25Gb/s super channel used per simulation

  // Total Blocks
  long long numBlocks_total;
  long long numBlocks_AR;
  long long numBlocks_IR;
  long long numBlocks_400;
  long long numBlocks_400AR;
  long long numBlocks_400IR;
  long long numBlocks_100;
  long long numBlocks_100AR;
  long long numBlocks_100IR;
  long long numBlocks_40;
  long long numBlocks_40AR;
  long long numBlocks_40IR;

  // Total Alloctions
  long long numAllocations_total;
  long long numAllocations_AR;
  long long numAllocations_IR;
  long long numAllocations_400;
  long long numAllocations_400AR;
  long long numAllocations_400IR;
  long long numAllocations_100;
  long long numAllocations_100AR;
  long long numAllocations_100IR;
  long long numAllocations_40;
  long long numAllocations_40AR;
  long long numAllocations_40IR;

  // Total Transponders
  long long numTransponders_total;
  long long numTransponders_AR;
  long long numTransponders_IR;
  long long numTransponders_400;
  long long numTransponders_400AR;
  long long numTransponders_400IR;
  long long numTransponders_100;
  long long numTransponders_100AR;
  long long numTransponders_100IR;
  long long numTransponders_40;
  long long numTransponders_40AR;
  long long numTransponders_40IR;

  // Average Transponders per Request
  double avgTransponders;
  double avgTransponders_AR;
  double avgTransponders_IR;
  double avgTransponders_400;
  double avgTransponders_400AR;
  double avgTransponders_400IR;
  double avgTransponders_100;
  double avgTransponders_100AR;
  double avgTransponders_100IR;
  double avgTransponders_40;
  double avgTransponders_40AR;
  double avgTransponders_40IR;

  // Total LPSs
  long long numLPSs_total;
  long long numLPSs_AR;
  long long numLPSs_IR;
  long long numLPSs_400;
  long long numLPSs_400AR;
  long long numLPSs_400IR;
  long long numLPSs_100;
  long long numLPSs_100AR;
  long long numLPSs_100IR;
  long long numLPSs_40;
  long long numLPSs_40AR;
  long long numLPSs_40IR;

  // Average LPS per Request
  double avgLPSs;
  double avgLPSs_AR;
  double avgLPSs_IR;
  double avgLPSs_400;
  double avgLPSs_400AR;
  double avgLPSs_400IR;
  double avgLPSs_100;
  double avgLPSs_100AR;
  double avgLPSs_100IR;
  double avgLPSs_40;
  double avgLPSs_40AR;
  double avgLPSs_40IR;

  // The bit rates for candidate super channels
  vector<int> candidateBR {25, 50, 100, 200, 400};

  /***  Debugging Variables ***/
  vector<int> blockedRequests; // The vector of IDs for blocked request
  long long   numRequests_AR;
  long long   numRequests_IR;
  long long   numRequests_400;
  long long   numRequests_400AR;
  long long   numRequests_400IR;
  long long   numRequests_100;
  long long   numRequests_100AR;
  long long   numRequests_100IR;
  long long   numRequests_40;
  long long   numRequests_40AR;
  long long   numRequests_40IR;

  /*** Variables for Traffic Generator ***/
  long long trafficGen_numRequests_400AR;
  long long trafficGen_numRequests_400IR;
  long long trafficGen_numRequests_100AR;
  long long trafficGen_numRequests_100IR;
  long long trafficGen_numRequests_40AR;
  long long trafficGen_numRequests_40IR;

private:
};

inline long long
double_precision(double num) {
  double eps = 0.0000000000005;
  num += eps;
  return (long long)(num * (1000 / TIMESLOT_LEN));
}
#endif
