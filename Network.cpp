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
Network::init()
{

  RoutingTable routingTable(this);
  routingTable.generate_routing_table();

  vector<bool>                 spectralSlotIndex;
  vector<vector<bool>>         coreIndex;
  vector<vector<vector<bool>>> successorIndex;

  for(unsigned int i = 0; i < numofNodes; i++)
  {
    for(unsigned int j = 0; j < numofNodes; j++)
    {
      for(unsigned int c = 0; c < numofCores; c++)
      {
        for(int k = 0; k < NUMOFSPECTRALSLOTS; k++)
        {
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

  firstMiliSec           = 0;
  lastMiliSec            = -1; // the reason for not 0 is for the first request.
  systemClock            = 0;
  requestCounter         = 0;
  numofDoneRequests      = 0;
  numofAllocatedRequests = 0;
  numofTransponders      = 0;
  numofSSs4Data          = 0;
  maxNumofTransponders   = 0;
  maxNumofSSs4Data       = 0;

  totalHoldingTime      = 0;
  totalTransponders     = 0;
  totalCoresUsed        = 0;
  totalLPSs             = 0;
  totalSSs4Data         = 0;
  totalSSsOccupied      = 0;
  totalMDataSize        = 0;
  avgExtFrag            = 0;
  avgIntFrag            = 0;
  avgHybridFrag         = 0;

  // Total Blocks
  totalBlocks         = 0;
  totalBlocks_AR      = 0;
  totalBlocks_IR      = 0;
  totalBlocks_400     = 0;
  totalBlocks_400AR   = 0;
  totalBlocks_400IR   = 0;
  totalBlocks_100     = 0;
  totalBlocks_100AR   = 0;
  totalBlocks_100IR   = 0;
  totalBlocks_40      = 0;
  totalBlocks_40AR    = 0;
  totalBlocks_40IR    = 0;

  // Total Transponders
  totalTransponders       = 0;
  totalTransponders_AR    = 0;
  totalTransponders_IR    = 0;
  totalTransponders_400   = 0;
  totalTransponders_400AR = 0;
  totalTransponders_400IR = 0;
  totalTransponders_100   = 0;
  totalTransponders_100AR = 0;
  totalTransponders_100IR = 0;
  totalTransponders_40    = 0;
  totalTransponders_40AR  = 0;
  totalTransponders_40IR  = 0;

  // Transponders per Request
  TpR       = 0;
  TpR_AR    = 0;
  TpR_IR    = 0;
  TpR_400   = 0;
  TpR_400AR = 0;
  TpR_400IR = 0;
  TpR_100   = 0;
  TpR_100AR = 0;
  TpR_100IR = 0;
  TpR_40    = 0;
  TpR_40AR  = 0;
  TpR_40IR  = 0;

  // Average Transponoders per Request
  TpR       = 0;
  TpR_AR    = 0;
  TpR_IR    = 0;
  TpR_400   = 0;
  TpR_400AR = 0;
  TpR_400IR = 0;
  TpR_100   = 0;
  TpR_100AR = 0;
  TpR_100IR = 0;
  TpR_40    = 0;
  TpR_40AR  = 0;
  TpR_40IR  = 0;

  // Total LPSs
  totalLPSs       = 0;
  totalLPSs_AR    = 0;
  totalLPSs_IR    = 0;
  totalLPSs_400   = 0;
  totalLPSs_400AR = 0;
  totalLPSs_400IR = 0;
  totalLPSs_100   = 0;
  totalLPSs_100AR = 0;
  totalLPSs_100IR = 0;
  totalLPSs_40    = 0;
  totalLPSs_40AR  = 0;
  totalLPSs_40IR  = 0;

  // Average LPS per Request
  LPSspR       = 0;
  LPSspR_AR    = 0;
  LPSspR_IR    = 0;
  LPSspR_400   = 0;
  LPSspR_400AR = 0;
  LPSspR_400IR = 0;
  LPSspR_100   = 0;
  LPSspR_100AR = 0;
  LPSspR_100IR = 0;
  LPSspR_40    = 0;
  LPSspR_40AR  = 0;
  LPSspR_40IR  = 0;

  // Light-segment Utilization Distribution
  numof400SC6 = 0; // number of 64QAM 400Gb/s super
                   // channel used per simulation
  numof400SC4 = 0; // number of 16QAM 400Gb/s super
                   // channel used per simulation
  numof400SC2 = 0; // number of QPSK 400Gb/s super
                   // channel used per simulation
  numof200SC6 = 0; // number of 64QAM 200Gb/s super
                   // channel used per simulation
  numof200SC4 = 0; // number of 16QAM 200Gb/s super
                   // channel used per simulation
  numof200SC2 = 0; // number of QPSK 200Gb/s super
                   // channel used per simulation
  numof100SC6 = 0; // number of 64QAM 100Gb/s super
                   // channel used per simulation
  numof100SC4 = 0; // number of 16QAM 100Gb/s super
                   // channel used per simulation
  numof100SC2 = 0; // number of QPSK 100Gb/s super
                   // channel used per simulation
  numof50SC6 = 0;  // number of 64QAM 50Gb/s super
                   // channel used per simulation
  numof50SC4 = 0;  // number of 16QAM 50Gb/s super
                   // channel used per simulation
  numof50SC2 = 0;  // number of QPSK 50Gb/s super channel
                   // used per simulation
  numof25SC = 0;   // number of 25Gb/s super channel used per simulation

  numofRequests_400 = (long long)(numofRequests * ratio_400);
  numofRequests_100   = (long long)(numofRequests * ratio_100);
  numofRequests_40  = numofRequests - numofRequests_400 - numofRequests_100;
  numofRequests_AR = numofRequests * AR_RATIO;
  numofRequests_IR = numofRequests - numofRequests_AR;

  trafficGen_numofRequests_AR = numofRequests_AR;
  trafficGen_numofRequests_IR  = numofRequests_IR;
  trafficGen_numofRequests_400 = numofRequests_400;
  trafficGen_numofRequests_100 = numofRequests_100;
  trafficGen_numofRequests_40  = numofRequests_40;

  totalAllocations_AR    = 0;
  totalAllocations_IR    = 0;
  totalAllocations_400   = 0;
  totalAllocations_400AR = 0;
  totalAllocations_400IR = 0;
  totalAllocations_100   = 0;
  totalAllocations_100AR = 0;
  totalAllocations_100IR = 0;
  totalAllocations_40    = 0;
  totalAllocations_40AR  = 0;
  totalAllocations_40IR  = 0;
}

void
Network::simulation()
{
  shared_ptr<EventQueue> eventQueue = make_shared<EventQueue>();
  TrafficGenerator   trafficGenerator(this, eventQueue);
  ResourceAssignment resourceAssignment(this, eventQueue);

  // Generate the first event
  systemClock = 0;
  trafficGenerator.gen_first_request();

  while(!eventQueue->ev_Queue.empty())
  {
#ifdef DEBUG_print_sequence_in_queue
    list<shared_ptr<Event>>::iterator iter;
    cout << "PRINT Event key properties in Queue" << endl;
    for(iter = eventQueue->ev_Queue.begin(); iter != eventQueue->ev_Queue.end();
        iter++)
    {
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
    if(systemClock <= event_ptr->eventTime)
    {
      systemClock = event_ptr->eventTime;
    }

    // Time Snapshot Abandoning
    long unsigned int cnt = systemClock * 1000 - firstMiliSec;
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
      while(cnt != 0)
      {
        spectralSlots.pop_front();
        cnt--;
      }

    if(event_ptr->eventType == c_Request)
    {
      shared_ptr<CircuitRequest> cr_ptr
          = static_pointer_cast<CircuitRequest>(event_ptr);
      resourceAssignment.handle_requests(cr_ptr);
      if(requestCounter != numofRequests)
        trafficGenerator.gen_request(systemClock);
    }
    else if(event_ptr->eventType == c_Release)
    {
      shared_ptr<CircuitRelease> cr_ptr
          = static_pointer_cast<CircuitRelease>(event_ptr);
      resourceAssignment.handle_releases(cr_ptr);
    }
    eventQueue->ev_Queue.pop_front(); // This will destroy the poped Event *.

    if(numofTransponders > maxNumofTransponders)
      maxNumofTransponders = numofTransponders;

    // Begin??? Does not have a good way of measuring
    if(numofSSs4Data > maxNumofSSs4Data)
    {
      maxNumofSSs4Data = numofSSs4Data;
    }
    // End???

#ifdef DEBUG_probe_NumofDoneReqeusts_and_NumofRequests
    cout << " " << NumofDoneRequests << " and " << NumofRequests << endl;
#endif

    if((totalBlocks + numofAllocatedRequests) == numofRequests)
      break;
  }


  cout << endl
       << "************************************************************"
       << endl;
#ifdef DEBUG_print_EventID_of_blocked_requests
  cout << "Start to print EventID of blocked reqeusts" << endl;
  for(long unsigned int i = 0; i < blockedRequests.size(); i++)
  {
    cout << blockedRequests[i] << ' ';
  }
  cout << endl;
#endif

  /*** Collect Measurement Metrics ***/
  // Begin??? Becuase of maxNumofSSs4Data
  spectrumUtilization = (double)maxNumofSSs4Data / NUMOFSPECTRALSLOTS;
  // End???

  avgHoldingTime      = totalHoldingTime / numofAllocatedRequests;
  avgCoresUsed        = (double)totalCoresUsed / numofAllocatedRequests;
  avgIntFrag = (1 - ((double)totalMDataSize / (totalSSs4Data * BW_SPECSLOT)));
  avgExtFrag          = (1 - (double)totalSSs4Data / totalSSsOccupied);
  avgHybridFrag
      = (1 - (double)totalMDataSize / (totalSSsOccupied * BW_SPECSLOT));

  // Average Metrics Computation
  TpR    = (double)totalTransponders / numofAllocatedRequests;
  TpR_AR = (double)totalTransponders_AR / totalAllocations_AR;
  TpR_IR = (double)totalTransponders_IR / totalAllocations_IR;
  TpR_400 = (double)totalTransponders_400 / totalAllocations_400;
  TpR_400AR = (double)totalTransponders_400AR / totalAllocations_400AR;
  TpR_400IR = (double)totalTransponders_400IR / totalAllocations_400IR;
  TpR_100   = (double)totalTransponders_100 / totalAllocations_100;
  TpR_100AR = (double)totalTransponders_100AR / totalAllocations_100AR;
  TpR_100IR = (double)totalTransponders_100IR / totalAllocations_100IR;
  TpR_40    = (double)totalTransponders_40 / totalAllocations_40;
  TpR_40AR  = (double)totalTransponders_40AR / totalAllocations_40AR;
  TpR_40IR  = (double)totalTransponders_40IR / totalAllocations_40IR;

  // LPSspR = (double)totalLPSs / numofAllocatedRequests;
  // LPSspR_400 = (double)totalLPSs_400 / (numofRequests * ratio_400);
  // LPSspR_400AR
  //     = (double)totalLPSs_400AR / (numofRequests * ratio_400 * AR_RATIO);
  // LPSspR_400IR
  //     = (double)totalLPSs_400IR / (numofRequests * ratio_400 * (1 -
  //     AR_RATIO));
  // LPSspR_100 = (double)totalLPSs_100 / (numofRequests * ratio_100);
  // LPSspR_100AR
  //     = (double)totalLPSs_100AR / (numofRequests * ratio_100 * AR_RATIO);
  // LPSspR_100IR
  //     = (double)totalLPSs_100IR / (numofRequests * ratio_100 * (1 -
  //     AR_RATIO));
  // LPSspR_40  = (double)totalLPSs_40 / (numofRequests * ratio_40);
  // LPSspR_40AR = (double)totalLPSs_40AR / (numofRequests * ratio_40 *
  // AR_RATIO); LPSspR_40IR
  //     = (double)totalLPSs_40IR / (numofRequests * ratio_40 * (1 - AR_RATIO));

  // cout << "Spectrum Utilization is: " << spectrumUtilization << endl;
  cout << "Number of Requests Breakdown: (AR, IR, 400, 100, 40)" << endl;
  cout << numofRequests_AR << ' ' << numofRequests_IR << ' '
       << numofRequests_400 << ' ' << numofRequests_100 << ' '
       << numofRequests_40 << ' ' << endl;
  cout << "Allocationed Requests Breakdown: (AR, IR, 400, 400AR, 400IR, 100, "
          "100AR, "
          "100IR, 40, 40AR, 40IR)"
       << endl;
  cout << totalAllocations_AR << ' ' << totalAllocations_IR << ' '
       << totalAllocations_400 << ' ' << totalAllocations_400AR << ' '
       << totalAllocations_400IR << ' ' << totalAllocations_100 << ' '
       << totalAllocations_100AR << ' ' << totalAllocations_100IR << ' '
       << totalAllocations_40 << ' ' << totalAllocations_40AR << ' '
       << totalAllocations_40IR << endl;
  cout << "Blocked Requests Breakdown: (AR, IR, 400, 400AR, 400IR, 100, 100AR, "
          "100IR, 40, 40AR, 40IR)"
       << endl;
  cout << totalBlocks_AR << ' ' << totalBlocks_IR << ' ' << totalBlocks_400
       << ' ' << totalBlocks_400AR << ' ' << totalBlocks_400IR << ' '
       << totalBlocks_100 << ' ' << totalBlocks_100AR << ' '
       << totalBlocks_100IR << ' ' << totalBlocks_40 << ' ' << totalBlocks_40AR
       << ' ' << totalBlocks_40IR << endl;
  cout << "Number of Transponders Used Breakdown: (AR, IR, 400, 400AR, 400IR, "
          "100, 100AR, 100IR, 40, 40AR, 40IR)"
       << endl;
  cout << totalTransponders_AR << ' ' << totalTransponders_IR << ' '
       << totalTransponders_400 << ' ' << totalTransponders_400AR << ' '
       << totalTransponders_400IR << ' ' << totalTransponders_100 << ' '
       << totalTransponders_100AR << ' ' << totalTransponders_100IR << ' '
       << totalTransponders_40 << ' ' << totalTransponders_40AR << ' '
       << totalTransponders_40IR << endl;
  cout << "Average Utilization of Transponders Breakdown: (AR, IR, 400, 400AR, "
          "400IR, 100, 100AR, 100IR, 40, 40AR, 40IR)"
       << endl;
  cout << TpR_AR << ' ' << TpR_IR << ' ' << TpR_400 << ' ' << TpR_400AR << ' '
       << TpR_400IR << ' ' << TpR_100 << ' ' << TpR_100AR << ' ' << TpR_100IR
       << ' ' << TpR_40 << ' ' << TpR_40AR << ' ' << TpR_40IR << endl;
  cout << "Utilization of LPSs breakdown: (AR, IR, 400, 400AR, 400IR, 100, "
          "100AR, 100IR, 40, 40AR, 40IR)"
       << endl;
  cout << totalLPSs_AR << ' ' << totalLPSs_IR << ' ' << totalLPSs_400 << ' '
       << totalLPSs_400AR << ' ' << totalLPSs_400IR << ' ' << totalLPSs_100
       << ' ' << totalLPSs_100AR << ' ' << totalLPSs_100IR << ' '
       << totalLPSs_40 << ' ' << totalLPSs_40AR << ' ' << totalLPSs_40IR
       << endl;
  /* cout << "Utilization of different SCs (400: 64QAM, 400: 16QAM, 400: QPSK, "
   */
  /*         "200: 64QAM, 200: 16QAM, 200: QPSK, 100: 64QAM, 100: 16QAM, 100: "
   */
  /*         "QPSK, 50: 64QAM, 50: 16QAM, 50: QPSK, 25): " */
  /*      << numof400SC6 << ' ' << numof400SC4 << ' ' << numof400SC2 << ' ' */
  /*      << numof200SC6 << ' ' << numof200SC4 << ' ' << numof200SC2 << ' ' */
  /*      << numof100SC6 << ' ' << numof100SC4 << ' ' << numof100SC2 << ' ' */
  /*      << numof50SC6 << ' ' << numof50SC4 << ' ' << numof50SC2 << ' ' */
  /*      << numof25SC << endl; */
  cout << "# of blocked requests is " << totalBlocks << endl;
  cout << "Blocking Probability: "
       << (double)totalBlocks / (double)numofRequests << endl;
  cout << "Max # of Transponders used: " << maxNumofTransponders << endl;
  cout << "Average Transponders Used per Request: " << TpR << endl;
  cout << "Average LPS per Request: " << LPSspR << endl;
  cout << "Network Load: " << lambda / mu << " Erlang" << endl;
  cout << "Average Cores Used Used per Request: " << avgCoresUsed << endl;
  cout << "Average Holding Time per Request: " << avgHoldingTime << endl;
  cout << "Average Internal Fragmentation: " << avgIntFrag << endl;
  cout << "Average External Fragmentation: " << avgExtFrag << endl;
  cout << "Average Hybrid Fragmentation: " << avgHybridFrag << endl;
}
