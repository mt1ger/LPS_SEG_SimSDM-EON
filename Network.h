#ifndef _NETWORK_H
#define _NETWORK_H

#include "EventQueue.h"
#include <map>
#include <memory>
#include <thread>
#include <vector>

#define TESTING

#ifdef TESTING
/** Not frequently needed **/
// #define DEBUG_print_sequence_in_queue // print event queue
// #define DEBUG_print_available_spectral_slots

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
#define BW_SPECSLOT        12.5
// #define AR_RATE 0.5
#define AR_RATE             0.5
#define MAX_NUMOFTIMESLICES 20

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

  /*** VARIABLES ***/
  // Inputted Variables
  char         fileName[500];
  double       lambda, mu;
  long long    numofRequests;
  unsigned int numofCores;
  int          maxAllowedSegments;

  // Topology Variables
  unsigned int                numofNodes;
  vector<vector<double>>      nodesWeight;
  vector<vector<vector<int>>> routingTable;

  // Global Tracker
  long long firstMiliSec;
  long long lastMiliSec;

  // Metrics per Request
  long long numofARs;
  long long numofIRs;
  double    totalHoldingTime;
  long long totalTranspondersUsed;
  long long totalCoresUsed;
  long long totalLPS;
  double    avgHoldingTime;
  double    avgTranspondersUsed;
  double    avgLPS;
  double    avgCoresUsed;
  double    spectrumUtilization;

  // Metrics to Measure Fregmentation
  double    totalMDataSize; // The total modulated data size.
  long long totalSS4Data;
  long long totalSSOccupied;

  double avgIntFrag;
  double avgExtFrag;
  double avgHybridFrag;

  // Timer and counters
  long long requestCounter; // Generated Number of Requests
  long long numofAllocatedRequests;
  long long numofDoneRequests;   // Requests which are successfully
                                 // allocated and released, or blocked
                                 // will be considered as DoneRequests
  long long numofFailedRequests; // Number of Requests that are blocked
  long long numofTransponders;   // Number of TranspondersUsed at a
                                 // Specific Time
  long long numofSS4Data;
  long long maxNumofTransponders; // For one time simulation, Maximum Number
                                  // of Transponders Used Simultaneously
  long long maxNumofSS4Data;      // For one time simulation, Maximum Number
                                  // of Data Spectral Slot Occupation
  int    numofSections;
  int    maxNumofSections;
  double systemClock;

  /****  Temp: Delete this after varified ****/
  long long probe_40  = 0;
  long long probe_100 = 0;
  long long probe_400 = 0;

  /**** Ratio of requests in different types ****/
  double ratio_40  = 0.3;
  double ratio_100 = 0.5;
  double ratio_400 = 0.2;

  /**** Number of requests in different types ****/
  long long request_40  = 0;
  long long request_100 = 0;
  long long request_400 = 0;

  /**** Probes for blocked requests in different types ****/
  long long block_40  = 0;
  long long block_100 = 0;
  long long block_400 = 0;

  // The bit rates for candidate super channels
  vector<int> candidateBR {25, 50, 100, 200, 400};

  /**** Super Channel Allocation Mapping ****/
  long long numof400SC6 = 0; // number of 64QAM 400Gb/s super
                             // channel used per simulation
  long long numof400SC4 = 0; // number of 16QAM 400Gb/s super
                             // channel used per simulation
  long long numof400SC2 = 0; // number of QPSK 400Gb/s super
                             // channel used per simulation
  long long numof200SC6 = 0; // number of 64QAM 200Gb/s super
                             // channel used per simulation
  long long numof200SC4 = 0; // number of 16QAM 200Gb/s super
                             // channel used per simulation
  long long numof200SC2 = 0; // number of QPSK 200Gb/s super
                             // channel used per simulation
  long long numof100SC6 = 0; // number of 64QAM 100Gb/s super
                             // channel used per simulation
  long long numof100SC4 = 0; // number of 16QAM 100Gb/s super
                             // channel used per simulation
  long long numof100SC2 = 0; // number of QPSK 100Gb/s super
                             // channel used per simulation
  long long numof50SC6 = 0;  // number of 64QAM 50Gb/s super
                             // channel used per simulation
  long long numof50SC4 = 0;  // number of 16QAM 50Gb/s super
                             // channel used per simulation
  long long numof50SC2 = 0;  // number of QPSK 50Gb/s super channel
                             // used per simulation
  long long numof25SC = 0; // number of 25Gb/s super channel used per simulation

  /**** Resource Variables ****/
  // The resource snapshot for a time point
  // [predecessor][successor][core][spectrum]
  vector<vector<vector<vector<bool>>>> resourceSnapshot;
  // The resource snapshots of all considered time points
  // [time][predecessor][successor][core][spectrum]
  list<vector<vector<vector<vector<bool>>>>> spectralSlots;

  // Debugging Variables
  vector<int> blockedRequests;

private:
};

#endif
