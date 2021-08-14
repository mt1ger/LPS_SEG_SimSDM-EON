#ifndef _NETWORK_H
#define _NETWORK_H

#include "EventQueue.h"
#include <map>
#include <memory>
#include <thread>
#include <vector>

#define TESTING
/* #define DEBUG_collect_eventID_of_blocked_requests */

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
#define MAX_NUMOFTIMESLICES 26
// #define MAX_NUMOFTIMESLICES 1

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
  long long    numofRequests;
  unsigned int numofCores;
  int          maxAllowedSegments;

  /* Topology Variables */
  unsigned int                numofNodes;
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
  long long numofAllocatedRequests;
  long long numofDoneRequests;    // Number of requests which are successfully
                                  // allocated, released, or blocked will be
                                  // considered as DoneRequests

  // Collect the number of transponders simultaneously used after
  // at the time slot for a request allocation
  long long numofTransponders;
  // Store the maximum number of transponders used simultaneously at any
  // time slot for one simlulation run
  long long maxNumofTransponders;
  long long numofSSs4Data; // Total number of data spectral slots occupied after
                           // a request allocation
  long long maxNumofSSs4Data; // Maximum number of data spectral slot occupied
                              // simultaneously

  // ???
  int numofSegments; // Total number of segments used after a request allocation
  int maxNumofSegments; // Maximum number of segments used simultaneously

  double    totalHoldingTime;
  long long totalCoresUsed;
  double    avgHoldingTime;
  double    avgCoresUsed;

  // Metrics to Measure Fregmentation
  double    spectrumUtilization;
  double    totalMDataSize; // The total modulated data size.
  long long totalSSs4Data;
  long long totalSSsOccupied;
  double    avgIntFrag;
  double    avgExtFrag;
  double    avgHybridFrag;

  /*** Break Down Data ***/
  // Light-segments Utilization Distribution
  long long numof400SC6; // number of 64QAM 400Gb/s super
                         // channel used per simulation
  long long numof400SC4; // number of 16QAM 400Gb/s super
                         // channel used per simulation
  long long numof400SC2; // number of QPSK 400Gb/s super
                         // channel used per simulation
  long long numof200SC6; // number of 64QAM 200Gb/s super
                         // channel used per simulation
  long long numof200SC4; // number of 16QAM 200Gb/s super
                         // channel used per simulation
  long long numof200SC2; // number of QPSK 200Gb/s super
                         // channel used per simulation
  long long numof100SC6; // number of 64QAM 100Gb/s super
                         // channel used per simulation
  long long numof100SC4; // number of 16QAM 100Gb/s super
                         // channel used per simulation
  long long numof100SC2; // number of QPSK 100Gb/s super
                         // channel used per simulation
  long long numof50SC6;  // number of 64QAM 50Gb/s super
                         // channel used per simulation
  long long numof50SC4;  // number of 16QAM 50Gb/s super
                         // channel used per simulation
  long long numof50SC2;  // number of QPSK 50Gb/s super channel
                         // used per simulation
  long long numof25SC;   // number of 25Gb/s super channel used per simulation

  // Total Blocks
  long long totalBlocks;
  long long totalBlocks_AR;
  long long totalBlocks_IR;
  long long totalBlocks_400;
  long long totalBlocks_400AR;
  long long totalBlocks_400IR;
  long long totalBlocks_100;
  long long totalBlocks_100AR;
  long long totalBlocks_100IR;
  long long totalBlocks_40;
  long long totalBlocks_40AR;
  long long totalBlocks_40IR;

  // Total Alloctions
  long long totalAllocations;
  long long totalAllocations_AR;
  long long totalAllocations_IR;
  long long totalAllocations_400;
  long long totalAllocations_400AR;
  long long totalAllocations_400IR;
  long long totalAllocations_100;
  long long totalAllocations_100AR;
  long long totalAllocations_100IR;
  long long totalAllocations_40;
  long long totalAllocations_40AR;
  long long totalAllocations_40IR;

  // Total Transponders
  long long totalTransponders;
  long long totalTransponders_AR;
  long long totalTransponders_IR;
  long long totalTransponders_400;
  long long totalTransponders_400AR;
  long long totalTransponders_400IR;
  long long totalTransponders_100;
  long long totalTransponders_100AR;
  long long totalTransponders_100IR;
  long long totalTransponders_40;
  long long totalTransponders_40AR;
  long long totalTransponders_40IR;

  // Average Transponders per Request
  double TpR;
  double TpR_AR;
  double TpR_IR;
  double TpR_400;
  double TpR_400AR;
  double TpR_400IR;
  double TpR_100;
  double TpR_100AR;
  double TpR_100IR;
  double TpR_40;
  double TpR_40AR;
  double TpR_40IR;

  // Total LPSs
  long long totalLPSs;
  long long totalLPSs_AR;
  long long totalLPSs_IR;
  long long totalLPSs_400;
  long long totalLPSs_400AR;
  long long totalLPSs_400IR;
  long long totalLPSs_100;
  long long totalLPSs_100AR;
  long long totalLPSs_100IR;
  long long totalLPSs_40;
  long long totalLPSs_40AR;
  long long totalLPSs_40IR;

  // Average LPS per Request
  double LPSspR;
  double LPSspR_AR;
  double LPSspR_IR;
  double LPSspR_400;
  double LPSspR_400AR;
  double LPSspR_400IR;
  double LPSspR_100;
  double LPSspR_100AR;
  double LPSspR_100IR;
  double LPSspR_40;
  double LPSspR_40AR;
  double LPSspR_40IR;

  // The bit rates for candidate super channels
  vector<int> candidateBR {25, 50, 100, 200, 400};

  /***  Debugging Variables ***/
  vector<int> blockedRequests; // The vector of IDs for blocked request
  long long   numofRequests_AR;
  long long   numofRequests_IR;
  long long   numofRequests_400;
  long long   numofRequests_100;
  long long   numofRequests_40;

  /*** Variables for Traffic Generator ***/
  long long trafficGen_numofRequests_AR;
  long long trafficGen_numofRequests_IR;
  long long trafficGen_numofRequests_400;
  long long trafficGen_numofRequests_100;
  long long trafficGen_numofRequests_40;

private:
};

#endif
