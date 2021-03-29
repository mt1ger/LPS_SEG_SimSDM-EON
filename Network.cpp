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
  numofFailedRequests    = 0;
  numofAllocatedRequests = 0;
  numofTransponders      = 0;
  numofSSs4Data          = 0;
  maxNumofTransponders   = 0;
  maxNumofSSs4Data       = 0;

  totalHoldingTime      = 0;
  totalTranspondersUsed = 0;
  totalCoresUsed        = 0;
  totalLPS              = 0;
  totalSSs4Data         = 0;
  totalSSsOccupied      = 0;
  totalMDataSize        = 0;
  avgExtFrag            = 0;
  avgIntFrag            = 0;
  avgHybridFrag         = 0;

  request_40  = ceil(numofRequests * ratio_40);
  request_100 = (long long)(numofRequests * ratio_100);
  request_400 = (long long)(numofRequests * ratio_400);

  numofARs = numofRequests * AR_RATE;
  numofIRs = numofRequests - numofARs;
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

    if((numofFailedRequests + numofAllocatedRequests) == numofRequests)
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
  avgTranspondersUsed = (double)totalTranspondersUsed / numofAllocatedRequests;
  avgCoresUsed        = (double)totalCoresUsed / numofAllocatedRequests;
  avgLPS              = (double)totalLPS / numofAllocatedRequests;
  avgIntFrag = (1 - ((double)totalMDataSize / (totalSSs4Data * BW_SPECSLOT)));
  avgExtFrag          = (1 - (double)totalSSs4Data / totalSSsOccupied);
  avgHybridFrag
      = (1 - (double)totalMDataSize / (totalSSsOccupied * BW_SPECSLOT));

  // cout << "Spectrum Utilization is: " << spectrumUtilization << endl;
  cout << "Utilization of different SCs (400: 64QAM, 400: 16QAM, 400: QPSK, "
          "200: 64QAM, 200: 16QAM, 200: QPSK, 100: 64QAM, 100: 16QAM, 100: "
          "QPSK, 50: 64QAM, 50: 16QAM, 50: QPSK, 25): "
       << numof400SC6 << ' ' << numof400SC4 << ' ' << numof400SC2 << ' '
       << numof200SC6 << ' ' << numof200SC4 << ' ' << numof200SC2 << ' '
       << numof100SC6 << ' ' << numof100SC4 << ' ' << numof100SC2 << ' '
       << numof50SC6 << ' ' << numof50SC4 << ' ' << numof50SC2 << ' '
       << numof25SC << endl;
  cout << "Max # of Transponders used: " << maxNumofTransponders << endl;
  cout << "# of blocked requests is " << numofFailedRequests << endl;
  cout << "Network Load: " << lambda / mu << " Erlang" << endl;
  cout << "Blocking Probability: "
       << (double)numofFailedRequests / (double)numofRequests << endl;
  cout << "Average Cores Used Used per Request: " << avgCoresUsed << endl;
  cout << "Average Transponders Used per Request: " << avgTranspondersUsed
       << endl;
  cout << "Average Holding Time per Request: " << avgHoldingTime << endl;
  cout << "Average LPS per Request: " << avgLPS << endl;
  cout << "Average Internal Fragmentation: " << avgIntFrag << endl;
  cout << "Average External Fragmentation: " << avgExtFrag << endl;
  cout << "Average Hybrid Fragmentation: " << avgHybridFrag << endl;
}
