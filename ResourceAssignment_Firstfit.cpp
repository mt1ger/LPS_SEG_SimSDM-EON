#include "ResourceAssignment_Firstfit.h"
#include <iostream>
#include <string>

/***** Check available spectral slots for the first link for this time
 * point *****/
void
ResourceAssignment::check_availability_first_link(
    unsigned int predecessor, unsigned int successor,
    list<vector<vector<vector<vector<bool>>>>>::iterator timeIter) {

  vector<int> availableSpecSlots_sec;
  availableSpecSlots.clear();
  for(unsigned int c = 0; c < network->numCores; c++) {
    for(int i = 0; i < NUMOFSPECTRALSLOTS; i++) {
      if(timeIter->at(predecessor).at(successor).at(c).at(i) == false) {
        availableSpecSlots_sec.push_back(c);
        availableSpecSlots_sec.push_back(i);
        availableSpecSlots.push_back(availableSpecSlots_sec);
        availableSpecSlots_sec.clear();
      }
    }
  }
#ifdef DEBUG_print_available_spectral_slots
  cout << "\033[0;32mPRINT\033[0m "
       << "available spectral slots (first link) for this time point:" << endl;
  list<vector<int>>::iterator i; // iterator to scan availableSpecSlots
  for(i = availableSpecSlots.begin(); i != availableSpecSlots.end(); i++) {
    for(long unsigned int j = 0; j < i->size(); j++) {
      cout << i->at(j) << ' ';
    }
    cout << "    ";
  }
  cout << endl;
#endif
}

/***** Check available spectral slots for the following links except for the
 * first link for this time point *****/
void
ResourceAssignment::check_availability_following_links(
    vector<int> *                                        circuitRoute_ptr,
    list<vector<vector<vector<vector<bool>>>>>::iterator timeIter) {

  list<vector<int>>::iterator i; // iterator to scan availableSpecSlots

  for(long unsigned int r = 2; r < circuitRoute_ptr->size(); r++) {
    for(i = availableSpecSlots.begin(); i != availableSpecSlots.end(); i++) {
      if(timeIter->at(circuitRoute_ptr->at(r - 1))
             .at(circuitRoute_ptr->at(r))
             .at(i->at(0))
             .at(i->at(1))
         == true) {
        i = availableSpecSlots.erase(i);
        i--; // Neutralize the i++
      }
    }
  }

#ifdef DEBUG_print_available_spectral_slots
  cout << "\033[0;32mPRINT\033[0m "
       << "available spectral slots (following links) for this time point:"
       << endl;
  for(i = availableSpecSlots.begin(); i != availableSpecSlots.end(); i++) {
    for(long unsigned int j = 0; j < i->size(); j++) {
      cout << i->at(j) << ' ';
    }
    cout << "    ";
  }
  cout << endl;
#endif
}

/***** Use the available spectral slots to form potential voids for this time
 * slot *****/
void
ResourceAssignment::create_voids(
    vector<int> *                                        circuitRoute_ptr,
    list<vector<vector<vector<vector<bool>>>>>::iterator timeIter) {
  list<vector<int>>::iterator i; // iterator to scan availableSpecSlots

  check_availability_first_link(circuitRoute_ptr->at(0),
                                circuitRoute_ptr->at(1), timeIter);
  check_availability_following_links(circuitRoute_ptr, timeIter);

  PotentialVoid potentialVoid;

  for(i = availableSpecSlots.begin(); i != availableSpecSlots.end(); i++) {
    if(potentialVoid.empty()) {
      if(i != (--availableSpecSlots.end())) {
        potentialVoid.core    = i->at(0);
        potentialVoid.startSS = i->at(1);
        potentialVoid.endSS   = i->at(1);
        potentialVoids.push_back(potentialVoid);
      }
    }
    else {
      if((i->at(1) == (potentialVoid.endSS + 1))
         && (i->at(0) == potentialVoid.core)) {
        potentialVoid.endSS       = i->at(1);
        *(--potentialVoids.end()) = potentialVoid;
      }
      else {
        if(potentialVoid.endSS != potentialVoid.startSS) {
          potentialVoid.clear();
          if(i != (--availableSpecSlots.end())) {
            potentialVoid.core    = i->at(0);
            potentialVoid.startSS = i->at(1);
            potentialVoid.endSS   = i->at(1);
            potentialVoids.push_back(potentialVoid);
          }
        }
        else {
          potentialVoid.clear();
          potentialVoids.erase(--potentialVoids.end());
          if(i != (--availableSpecSlots.end())) {
            potentialVoid.core    = i->at(0);
            potentialVoid.startSS = i->at(1);
            potentialVoid.endSS   = i->at(1);
            potentialVoids.push_back(potentialVoid);
          }
        }
      }
    }
  }

#ifdef DEBUG_print_potential_voids
  cout << "\033[0;32mPRINT\033[0m "
       << "Potential voids for this time slot" << endl;

  auto m = potentialVoids.begin();
  cout << "  ";
  for(m = potentialVoids.begin(); m != potentialVoids.end(); m++) {
    cout << m->core << ' ' << m->startSS << ' ' << m->endSS << "     ";
  }
  cout << endl;
#endif
}

/**** Check the availability of an assignedSpectralSegment for a specific time
 * slot on the lightpath ****/
void
ResourceAssignment::check_resource_availibility(
    list<vector<vector<vector<vector<bool>>>>>::iterator
                 iter, // lterator for network->spectralSlots
    vector<int> &circuitRoute, bool *tempAvailableFlag) {
  auto assignedSpectralSegments_iter = assignedSpectralSegments.begin();
  for(; assignedSpectralSegments_iter != assignedSpectralSegments.end();
      assignedSpectralSegments_iter++) {
    for(long unsigned int i = 1; i < circuitRoute.size(); i++) {
      for(int k = assignedSpectralSegments_iter->firstSS;
          k <= assignedSpectralSegments_iter->lastSS; k++) {
        if(iter->at(circuitRoute[i - 1])
               .at(circuitRoute[i])
               .at(assignedSpectralSegments_iter->core)
               .at(k)
           == true) {
          *tempAvailableFlag = false;
          break;
        }
      }
      if((*tempAvailableFlag) == false)
        break;
    }
  }
}

void
ResourceAssignment::super_channel_info(
    ModulationFormats *modulationFormats_ptr, vector<int> *circuitRoute_ptr,
    shared_ptr<CircuitRequest> circuitRequest_ptr) {

  for(long unsigned int i = 0; i < network->candidateBR.size(); i++) {
    SuperChannel sC;
    sC.sCBitRate = network->candidateBR.at(i);
    candidateSCs.push_back(sC);
  }

  for(long unsigned int i = 0; i < candidateSCs.size(); i++) {
    modulationFormats_ptr->mf_chosen(
        circuitRoute_ptr, &(candidateSCs[i].numofMSSs),
        &(candidateSCs[i].sCBitRate), &(candidateSCs[i].mF),
        &(candidateSCs[i].mFBitsperSignal));

    // 0 means no availability for this super channel option
    if(candidateSCs[i].numofMSSs != 0) {
      candidateSCs[i].numofSCs4Request = ceil(
          (double)circuitRequest_ptr->bitRate / candidateSCs[i].sCBitRate);
      candidateSCs[i].numofMSSs4Request
          = (candidateSCs[i].numofMSSs + GB) * candidateSCs[i].numofSCs4Request;
    }
  }
#ifdef DEBUG_print_SC_info
  cout << "\033[0;32mPRINT\033[0m "
       << "SC related info" << endl;
  for(long unsigned int i = 0; i < candidateSCs.size(); i++) {
    cout << "  ";
    cout << "BR4SC: " << candidateSCs[i].sCBitRate
         << "    MF: " << candidateSCs[i].mF
         << "    SSs4SC: " << candidateSCs[i].numofMSSs
         << "    SCs4Request: " << candidateSCs[i].numofSCs4Request
         << "    SSs4request: " << candidateSCs[i].numofMSSs4Request << endl;
  }
#endif
}

void
ResourceAssignment::sort_super_channel(
    ModulationFormats *modulationFormats_ptr, vector<int> *circuitRoute_ptr,
    shared_ptr<CircuitRequest> circuitRequest_ptr,
    SuperChannelQueue *        superChannelQueue_ptr) {

  super_channel_info(modulationFormats_ptr, circuitRoute_ptr,
                     circuitRequest_ptr);
  for(long unsigned int i = 0; i < candidateSCs.size(); i++) {
    if(candidateSCs[i].numofMSSs == 0)
      continue;
    else
      superChannelQueue_ptr->insert(&candidateSCs[i]);
  }
  bool *availableFlag_ptr;

#ifdef DEBUG_print_sorted_SC
  cout << "\033[0;32mPRINT\033[0m "
       << "Sorted super channels:" << endl;
  superChannelQueue_ptr->print_BR();
#endif
}

int
ResourceAssignment::match_SC_and_voids(unsigned int bitRate,
                                       bool *       availableFlag_ptr,
                                       list<SuperChannel>::iterator sCIter,
                                       unsigned int *numofGB_ptr) {

  SpectrumSegment spectrumSegment;
  auto             potentialVoids_iter = potentialVoids.begin();
  while(bitRate > 0) {
    /* Maybe useful when using multiple SC options for allocation */
    /* if(potentialVoids_iter == potentialVoids.end()) { */
    /*   if(i != 0) */
    /*     NextSC = true; */
    /*   else */
    /*     NextSC = false; */
    /*   break; */
    /* } */
    if(potentialVoids_iter == potentialVoids.end()) {
      *availableFlag_ptr = false;
      break;
    }
    else if(potentialVoids_iter->endSS - potentialVoids_iter->startSS + 1
            == sCIter->numofMSSs + GB) {
      spectrumSegment.core    = potentialVoids_iter->core;
      spectrumSegment.firstSS = potentialVoids_iter->startSS;
      spectrumSegment.lastSS  = potentialVoids_iter->endSS;
      spectrumSegment.mF      = sCIter->mF;
      spectrumSegment.bitRate = sCIter->sCBitRate;
      /* May need a function HERE to record the used SC option when using
       * multiple SC options for allocation.
       * assignedSpectralSegments_sec.push_back(indication of the SC
       * option) as assignedSpectralSegments_sec[3]*/
      assignedSpectralSegments.push_back(spectrumSegment);
      if(bitRate >= sCIter->sCBitRate)
        bitRate -= sCIter->sCBitRate;
      else
        bitRate = 0;
      (*numofGB_ptr)++;
      potentialVoids_iter = potentialVoids.erase(potentialVoids_iter);

#ifdef DEBUG_print_potential_voids
      cout << "\033[0;32mThe SC is \033[0m " << sCIter->sCBitRate << endl;
      auto probeIndex = potentialVoids.begin();
      cout << "\033[0;32mPRINT\033[0m "
           << "Potential voids AFTER reservation" << endl;
      for(probeIndex = potentialVoids.begin();
          probeIndex != potentialVoids.end(); probeIndex++) {
        cout << probeIndex->core << ' ' << probeIndex->startSS << ' '
             << probeIndex->endSS << "     ";
      }
      cout << endl;
#endif

      // break;
      continue;
    }
    else if(potentialVoids_iter->endSS - potentialVoids_iter->startSS + 1
            > sCIter->numofMSSs + GB) {
      spectrumSegment.core    = potentialVoids_iter->core;
      spectrumSegment.firstSS = potentialVoids_iter->startSS;
      spectrumSegment.lastSS
          = potentialVoids_iter->startSS + sCIter->numofMSSs + GB - 1;
      spectrumSegment.mF      = sCIter->mF;
      spectrumSegment.bitRate = sCIter->sCBitRate;
      /* May need a function HERE to record the used SC option when using
       * multiple SC options for allocation.
       * assignedSpectralSegments_sec.push_back(indication of the SC
       * option) as assignedSpectralSegments_sec[3]*/
      assignedSpectralSegments.push_back(spectrumSegment);
      if(bitRate >= sCIter->sCBitRate)
        bitRate -= sCIter->sCBitRate;
      else
        bitRate = 0;
      if(potentialVoids_iter->endSS
             - (potentialVoids_iter->startSS + sCIter->numofMSSs + GB) + 1
         > GB) {
        potentialVoids_iter->startSS
            = potentialVoids_iter->startSS + sCIter->numofMSSs + GB;
      }
      else {
        potentialVoids_iter = potentialVoids.erase(potentialVoids_iter);
      }
      (*numofGB_ptr)++;

#ifdef DEBUG_print_potential_voids
      cout << "\033[0;32mThe SC is \033[0m " << sCIter->sCBitRate << endl;
      auto probeIndex = potentialVoids.begin();
      cout << "\033[0;32mPRINT\033[0m "
           << "Potential voids AFTER reservation" << endl;
      cout << "  ";
      for(probeIndex = potentialVoids.begin();
          probeIndex != potentialVoids.end(); probeIndex++) {
        cout << probeIndex->core << ' ' << probeIndex->startSS << ' '
             << probeIndex->endSS << "     ";
      }
      cout << endl;
#endif
      // break;
      continue;
    }
    else if((potentialVoids_iter->endSS - potentialVoids_iter->startSS + 1)
            < sCIter->numofMSSs + GB) {
      potentialVoids_iter++;
    }
  }
  return bitRate;
}

void
ResourceAssignment::enlarge_time_snapshot(
    list<vector<vector<vector<vector<bool>>>>> &timeSnapshots,
    long long                                   releaseTime) {

  if(releaseTime > network->lastMiliSec) {
    long long cnt = releaseTime - network->lastMiliSec - 1;
    while(cnt != 0) {
      network->spectralSlots.push_back(network->resourceSnapshot);
      cnt--;
    }
    network->lastMiliSec
        = releaseTime - 1; // The size of (0 to sometime - 1) is sometime.
  }
}

long long
ResourceAssignment::precision_double(double num) {
  double eps = 0.000005;
  num += eps;
  return floor(num * 1000);
}

void
ResourceAssignment::handle_requests(
    shared_ptr<CircuitRequest> circuitRequest_ptr) {

  RoutingTable routingTable(network);
  vector<int>  circuitRoute; // store the nodes on the path
  // Indicates is the selected slicing plan can be satisfied
  bool         availableFlag     = false;
  string       mF                = "BPSK";
  unsigned int bR                = 0;
  unsigned int mFBitsperSignal   = 0;
  bool         tempAvailableFlag = true; // flag just for handling duplicates
  long long    advTime;
  unsigned int numofGB; // number of SEGs

  /** Find shortest path **/
  circuitRoute = routingTable.get_shortest_path(circuitRequest_ptr->src,
                                                circuitRequest_ptr->dest);
  /** Append time slots for the LPR's duration that exceeds the
   * network->spectralSlots **/
  list<vector<vector<vector<vector<bool>>>>>::iterator timeIter;
  long long startTime   = precision_double(circuitRequest_ptr->startTime);
  long long releaseTime = precision_double(circuitRequest_ptr->releaseTime);
  long long duration    = precision_double(circuitRequest_ptr->duration);
  enlarge_time_snapshot(network->spectralSlots, releaseTime);

  /** Skip idle time slots **/
  timeIter = network->spectralSlots.begin();
  advTime  = startTime - network->firstMiliSec;
  advance(timeIter, advTime);

  // Stores attributes for all periods of time slots for LPSs
  // [number of LPSs][number of light-segment for the same period]
  // [{start_time, end_time, core, first_SS, last_SS}]
  vector<vector<LightSegment>> LSAttributes;
  // [number of light-segment for the same period]
  // [{start_time, end_time, core, first_SS, last_SS}]
  vector<LightSegment> LSAttributes_sec;

  /** Compute supported modulation formats **/
  ModulationFormats modulationFormats(circuitRequest_ptr, network);
  modulationFormats.mf_chosen(
      &circuitRoute, &circuitRequest_ptr->occupiedSpectralSlots,
      &circuitRequest_ptr->bitRate, &mF, &mFBitsperSignal);
  circuitRequest_ptr->occupiedSpectralSlots
      = circuitRequest_ptr->occupiedSpectralSlots + GB; // For data + GB
  bR = circuitRequest_ptr->bitRate;
  if(bR == 40)
    bR = 50;

  /** Process slicing plans **/
  SuperChannelQueue superChannelQueue;
  candidateSCs.clear();
  sort_super_channel(&modulationFormats, &circuitRoute, circuitRequest_ptr,
                     &superChannelQueue);
  // Delete the slicing plan that exceeds maxAllowedSegments
  auto iter = superChannelQueue.begin();
  for(; iter != superChannelQueue.end(); iter++) {
    SuperChannel *sC_ptr = superChannelQueue.get(iter);
    if(sC_ptr->numofSCs4Request > network->maxLightSegments) {
      iter = superChannelQueue.erase(iter);
      iter--; // This iter-- is used to neutralize iter++
    }
  }
#ifdef DEBUG_print_sorted_SC
  cout << "\033[0;32mPRINT\033[0m "
       << "Sorted super channels (delete SCs that exceeds SEG limitation):"
       << endl;
  superChannelQueue.print_BR();
#endif

  /** Reset related parameters **/
  assignedSpectralSegments.clear();
  auto durationCount = duration; // discrete duration count in ms
  auto timeCount     = startTime;

  /** Considered LPR's duration time slot by time slot **/
  // The logic of SEG based on the truth that availableFlag is true
  availableFlag = true;
  for(; durationCount != 0; timeIter++) {
    /* Handle duplicates */
    // Indicates if the light-segments used for previous time slot can be
    // satisfied
    tempAvailableFlag = true;
    // Re-use the light-segments for the previous time slot
    if(!assignedSpectralSegments.empty()) {
      check_resource_availibility(timeIter, circuitRoute, &tempAvailableFlag);
      if(tempAvailableFlag == true) {
        for(int i = 0; i < LSAttributes_sec.size(); i++) {
          LSAttributes_sec[i].endTimeSlot = timeCount;
        }
        if(durationCount == 1) {
          LSAttributes.push_back(LSAttributes_sec);
        }
        timeCount++;
        durationCount--;
        continue;
      }
      else {
        LSAttributes.push_back(LSAttributes_sec);
      }
    }

    /* Processing for new period */
#ifdef DEBUG_print_potential_voids
    cout << "\033[0;32m**** CHANGE PERIOD ****\033[0m " << endl;
#endif
    // Set the starting time for the new period
    auto snapshotStartTime = timeCount;

    // Clear the buffer that stores the light-segments used for last time slot
    // assignedSpectralSegments.clear();

    // Find all the voids for this time point
    potentialVoids.clear();
    create_voids(&circuitRoute, timeIter);
    potentialVoids_backup = potentialVoids;

    // No voids
    if(potentialVoids.empty()) {
      availableFlag = false;
      break;
    }

    // No available slicing plans
    if(superChannelQueue.empty()) {
      availableFlag = false;
      break;
    }

    unsigned int bitRate = circuitRequest_ptr->bitRate;
    auto         sCIter  = superChannelQueue.begin();
    auto         recordIter = superChannelQueue.begin();
    for(; sCIter != superChannelQueue.end(); sCIter++) {
      availableFlag = true;
      assignedSpectralSegments.clear();
      potentialVoids = potentialVoids_backup;

#ifdef DEBUG_print_potential_voids
      cout << "\033[0;32mPRINT\033[0m "
           << "Potential voids for this time slot, before pre-allocation"
           << endl;

      auto m = potentialVoids.begin();
      cout << "  ";
      for(m = potentialVoids.begin(); m != potentialVoids.end(); m++) {
        cout << m->core << ' ' << m->startSS << ' ' << m->endSS << "     ";
      }
      cout << endl;
#endif

      match_SC_and_voids(bitRate, &availableFlag, sCIter, &numofGB);
      if(availableFlag == true) {
        if(sCIter != recordIter) {
          /* cout << "changed lightSegment type" << endl; */
          recordIter = sCIter;
        }
        break;
      }
    }

    if(availableFlag == true) {
      LSAttributes_sec.clear();
      for(auto iter = assignedSpectralSegments.begin();
          iter != assignedSpectralSegments.end(); iter++) {
        LightSegment lightSegment;
        lightSegment.startTimeSlot = snapshotStartTime;
        lightSegment.core          = iter->core;
        lightSegment.firstSS       = iter->firstSS;
        lightSegment.lastSS        = iter->lastSS;
        lightSegment.bitRate       = iter->bitRate;
        lightSegment.mF            = iter->mF;
        LSAttributes_sec.push_back(lightSegment);
      }
    }
    if(availableFlag == false)
      break;

    for(int i = 0; i < LSAttributes_sec.size(); i++) {
      LSAttributes_sec[i].endTimeSlot = timeCount;
    }
    if(durationCount == 1) {
      LSAttributes.push_back(LSAttributes_sec);
    }
    timeCount++;
    durationCount--;
  }
  if(LSAttributes.size() > network->maxTimeSlices)
    availableFlag = false;

  /** Real allocation and printout **/
  if(availableFlag == false) {

    network->numDoneRequests++;

#ifdef DEBUG_collect_eventID_of_blocked_requests
    network->blockedRequests.push_back(circuitRequest_ptr->eventID);
#endif
#ifdef PRINT_allocation_block_release
    cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
         << endl;
    cout << "Blocked Request ID: " << circuitRequest_ptr->eventID << endl;
    cout << "Totally performed " << LSAttributes.size() << " LPSs." << endl;
    cout << "Modulation Format: " << mF << endl;
    cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
         << endl;
#endif
    assignedSpectralSegments.clear();

    network->totalBlocks++;
    switch(circuitRequest_ptr->bitRate) {
      case 40:
        network->totalBlocks_40++;
        if(circuitRequest_ptr->requestType == c_AR) {
          network->totalBlocks_AR++;
          network->totalLPSs_AR++;
          network->totalBlocks_40AR++;
        }
        else {
          network->totalBlocks_IR++;
          network->totalLPSs_IR++;
          network->totalBlocks_40IR++;
        }
        break;
      case 100:
        network->totalBlocks_100++;
        if(circuitRequest_ptr->requestType == c_AR) {
          network->totalBlocks_AR++;
          network->totalLPSs_AR++;
          network->totalBlocks_100AR++;
        }
        else {
          network->totalBlocks_IR++;
          network->totalLPSs_IR++;
          network->totalBlocks_100IR++;
        }
        break;
      case 400:
        network->totalBlocks_400++;
        if(circuitRequest_ptr->requestType == c_AR) {
          network->totalBlocks_AR++;
          network->totalLPSs_AR++;
          network->totalBlocks_400AR++;
        }
        else {
          network->totalBlocks_IR++;
          network->totalLPSs_IR++;
          network->totalBlocks_400IR++;
        }
        break;
    }
  }
  else if(availableFlag == true) {
    timeIter = network->spectralSlots.begin();

    // Occupy SSs
    for(long unsigned int i = 0; i < LSAttributes.size(); i++) {
      startTime = LSAttributes[i][0].startTimeSlot;
      timeIter  = network->spectralSlots.begin();
      advTime   = startTime - network->firstMiliSec;
      advance(timeIter, advTime);
      durationCount = LSAttributes[i][0].endTimeSlot
                      - LSAttributes[i][0].startTimeSlot + 1;

      for(long unsigned int j = 0; j < LSAttributes[i].size(); j++) {
        for(; durationCount != 0; timeIter++) {
          for(long unsigned int r = 1; r < circuitRoute.size(); r++) {
            for(int k = LSAttributes[i][j].firstSS;
                k < LSAttributes[i][j].lastSS + 1; k++) {
              timeIter->at(circuitRoute[r - 1])
                  .at(circuitRoute[r])
                  .at(LSAttributes[i][j].core)
                  .at(k)
                  = true;
            }
          }
          durationCount--;
        }
      }
    }

    /** Compute the maximum utilization of segments for this LPR **/
    unsigned long int maxLightSegments
        = 0; // Maximum number of segments this LPR used
    for(int i = 0; i < LSAttributes.size(); i++) {
      if(LSAttributes.at(i).size() > maxLightSegments) {
        maxLightSegments = LSAttributes.at(i).size();
      }
    }

#ifdef PRINT_allocation_block_release
    cout << "------------------------------------------------------------"
         << endl;
    cout << "Request ID: " << circuitRequest_ptr->eventID
         << "\tArrival: " << circuitRequest_ptr->arrivalTime
         << "\tStart: " << circuitRequest_ptr->startTime << "\tEnd: "
         << circuitRequest_ptr->startTime + circuitRequest_ptr->duration
         << endl;
    cout << "Source: " << circuitRequest_ptr->src
         << "  Destination: " << circuitRequest_ptr->dest << endl;
    cout << "Path: ";
    for(unsigned int t = 0; t < circuitRoute.size() - 1; t++)
      cout << circuitRoute.at(t) << " --> ";
    cout << circuitRoute.at(circuitRoute.size() - 1) << endl;
    // cout << "Super Channel Option: " << bR << endl;
    ;
    cout << "Totally performed " << LSAttributes.size() - 1 << " LPSs." << endl;
    for(long unsigned int i = 0; i < LSAttributes.size(); i++) {
      cout << "From " << LSAttributes[i][0].startTimeSlot << " to "
           << LSAttributes[i][0].endTimeSlot << endl;
      for(long unsigned int j = 0; j < LSAttributes[i].size(); j++) {
        cout << "  ";
        cout << "Core: " << LSAttributes[i][j].core
             << "  Spectral Segments: " << LSAttributes[i][j].firstSS << " to "
             << LSAttributes[i][j].lastSS
             << "  light-segment: " << LSAttributes[i][j].bitRate
             << "  MF: " << LSAttributes[i][j].mF << endl;
      }
    }
    cout << "# of Segments: " << maxLightSegments << endl;
    cout << "# of Time-Slices: " << LSAttributes.size() << endl;
    cout << "------------------------------------------------------------"
         << endl;
#endif
    assignedSpectralSegments.clear();

    shared_ptr<Event> event_ptr;
    event_ptr = make_shared<CircuitRelease>(
        circuitRequest_ptr->eventID, circuitRoute,
        circuitRequest_ptr->arrivalTime, circuitRequest_ptr->startTime,
        circuitRequest_ptr->duration,
        circuitRequest_ptr->startTime + circuitRequest_ptr->duration,
        maxLightSegments);
    eventQueue->queue_insert(event_ptr);

    switch(circuitRequest_ptr->bitRate) {
      case 40:
        network->totalLPSs_40 += LSAttributes.size() - 1;
        network->totalTransponders_40 += maxLightSegments;
        network->totalAllocations_40++;
        if(circuitRequest_ptr->requestType == c_AR) {
          network->totalAllocations_40AR++;
          network->totalAllocations_AR++;
          network->totalTransponders_AR += maxLightSegments;
          network->totalTransponders_40AR += maxLightSegments;
          network->totalLPSs_AR += LSAttributes.size() - 1;
          network->totalLPSs_40AR += LSAttributes.size() - 1;
        }
        else {
          network->totalAllocations_40IR++;
          network->totalAllocations_IR++;
          network->totalTransponders_IR += maxLightSegments;
          network->totalTransponders_40IR += maxLightSegments;
          network->totalLPSs_IR += LSAttributes.size() - 1;
          network->totalLPSs_40IR += LSAttributes.size() - 1;
        }
        break;
      case 100:
        network->totalTransponders_100 += maxLightSegments;
        network->totalLPSs_100 += LSAttributes.size() - 1;
        network->totalAllocations_100++;
        if(circuitRequest_ptr->requestType == c_AR) {
          network->totalAllocations_100AR++;
          network->totalAllocations_AR++;
          network->totalTransponders_AR += maxLightSegments;
          network->totalTransponders_100AR += maxLightSegments;
          network->totalLPSs_AR += LSAttributes.size() - 1;
          network->totalLPSs_100AR += LSAttributes.size() - 1;
        }
        else {
          network->totalAllocations_100IR++;
          network->totalAllocations_IR++;
          network->totalTransponders_IR += maxLightSegments;
          network->totalTransponders_100IR += maxLightSegments;
          network->totalLPSs_IR += LSAttributes.size() - 1;
          network->totalLPSs_100IR += LSAttributes.size() - 1;
        }
        break;
      case 400:
        network->totalTransponders_400 += maxLightSegments;
        network->totalLPSs_400 += LSAttributes.size() - 1;
        network->totalAllocations_400++;
        if(circuitRequest_ptr->requestType == c_AR) {
          network->totalAllocations_400AR++;
          network->totalAllocations_AR++;
          network->totalTransponders_AR += maxLightSegments;
          network->totalTransponders_400AR += maxLightSegments;
          network->totalLPSs_AR += LSAttributes.size() - 1;
          network->totalLPSs_400AR += LSAttributes.size() - 1;
        }
        else {
          network->totalAllocations_400IR++;
          network->totalAllocations_IR++;
          network->totalTransponders_IR += maxLightSegments;
          network->totalTransponders_400IR += maxLightSegments;
          network->totalLPSs_IR += LSAttributes.size() - 1;
          network->totalLPSs_400IR += LSAttributes.size() - 1;
        }
        break;
    }

    // DOES NOT WORK
    if(bR == 25)
      network->num25SC += LSAttributes[0].size();
    else if(bR == 50) {
      if(mF == "QPSK")
        network->num50SC2 += LSAttributes[0].size();
      else if(mF == "16QAM")
        network->num50SC4 += LSAttributes[0].size();
      else if(mF == "64QAM")
        network->num50SC6 += LSAttributes[0].size();
    }
    else if(bR == 100) {
      if(mF == "QPSK")
        network->num100SC2 += LSAttributes[0].size();
      else if(mF == "16QAM")
        network->num100SC4 += LSAttributes[0].size();
      else if(mF == "64QAM")
        network->num100SC6 += LSAttributes[0].size();
    }
    else if(bR == 200) {
      if(mF == "QPSK")
        network->num200SC2 += LSAttributes[0].size();
      else if(mF == "16QAM")
        network->num200SC4 += LSAttributes[0].size();
      else if(mF == "64QAM")
        network->num200SC6 += LSAttributes[0].size();
    }
    else if(bR == 400) {
      if(mF == "QPSK")
        network->num400SC2 += LSAttributes[0].size();
      else if(mF == "16QAM")
        network->num400SC4 += LSAttributes[0].size();
      else if(mF == "64QAM")
        network->num400SC6 += LSAttributes[0].size();
    }

    network->numAllocatedRequests++;
    network->numTransponders += maxLightSegments;
    // network->numSSs4Data
    //     = (LSAttributes[0][0][4] - LSAttributes[0][0][3] + 1 -
    //     GB)
    //       * LSAttributes[0].size();
    network->totalTransponders += maxLightSegments;
    network->totalHoldingTime += circuitRequest_ptr->duration;
    // network->totalCoresUsed++;
    network->totalLPSs += LSAttributes.size() - 1;
    // network->totalSSs4Data += (LSAttributes[0][0][4]
    //                            - LSAttributes[0][0][3] + 1 - GB)
    //                           * LSAttributes[0].size();
    // network->totalMDataSize += network->totalSSsOccupied
    //     += (LSAttributes[0][0][4] - LSAttributes[0][0][3] + 1)
    //        * LSAttributes[0].size();
  }
}

void
ResourceAssignment::handle_releases(
    shared_ptr<CircuitRelease> circuitRelease_ptr) {

  network->numDoneRequests++;
  network->numTransponders -= circuitRelease_ptr->transpondersUsed;

#ifdef PRINT_allocation_block_release
  cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
       << endl;
  cout << "Release Event ID: " << circuitRelease_ptr->eventID
       << "\tTime: " << circuitRelease_ptr->eventTime << endl;
  cout << "Number of transpondersUsed " << network->numTransponders << endl;
  cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
       << endl;
#endif
}

