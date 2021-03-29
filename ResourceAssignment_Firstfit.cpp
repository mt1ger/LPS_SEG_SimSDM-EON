/**************************************************
 * First-Fit
 **************************************************/

#include "ResourceAssignment_Firstfit.h"
#include <iostream>
#include <string>

/***** Check available spectral slots for the first link for this time point
 *****/
void
ResourceAssignment::check_availability_first_link(
    unsigned int predecessor, unsigned int successor,
    list<vector<vector<vector<vector<bool>>>>>::iterator timeIter)
{
  vector<int> availableSpecSlots_sec;

  availableSpecSlots.clear();
  for(unsigned int c = 0; c < network->numofCores; c++)
  {
    for(int i = 0; i < NUMOFSPECTRALSLOTS; i++)
    {
      if(timeIter->at(predecessor).at(successor).at(c).at(i) == false)
      {
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
  for(i = availableSpecSlots.begin(); i != availableSpecSlots.end(); i++)
  {
    for(long unsigned int j = 0; j < i->size(); j++)
    {
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
    list<vector<vector<vector<vector<bool>>>>>::iterator timeIter)
{
  list<vector<int>>::iterator i; // iterator to scan availableSpecSlots

  for(long unsigned int r = 2; r < circuitRoute_ptr->size(); r++)
  {
    for(i = availableSpecSlots.begin(); i != availableSpecSlots.end(); i++)
    {
      if(timeIter->at(circuitRoute_ptr->at(r - 1))
             .at(circuitRoute_ptr->at(r))
             .at(i->at(0))
             .at(i->at(1))
         == true)
      {
        i = availableSpecSlots.erase(i);
        i--; // Neutralize the i++
      }
    }
  }

#ifdef DEBUG_print_available_spectral_slots
  cout << "\033[0;32mPRINT\033[0m "
       << "available spectral slots (following links) for this time point:"
       << endl;
  for(i = availableSpecSlots.begin(); i != availableSpecSlots.end(); i++)
  {
    for(long unsigned int j = 0; j < i->size(); j++)
    {
      cout << i->at(j) << ' ';
    }
    cout << "    ";
  }
  cout << endl;
#endif
}

/***** Use the available spectral slots to form potential voids for this time
 * point *****/
void
ResourceAssignment::create_voids(
    vector<int> *                                        circuitRoute_ptr,
    list<vector<vector<vector<vector<bool>>>>>::iterator timeIter)
{
  list<vector<int>>::iterator i; // iterator to scan availableSpecSlots
  vector<unsigned int>        potentialVoids_sec;

  check_availability_first_link(circuitRoute_ptr->at(0),
                                circuitRoute_ptr->at(1), timeIter);
  check_availability_following_links(circuitRoute_ptr, timeIter);

  for(i = availableSpecSlots.begin(); i != availableSpecSlots.end(); i++)
  {
    if(potentialVoids_sec.empty())
    {
      if(i != (--availableSpecSlots.end()))
      {
        potentialVoids_sec.push_back(i->at(0));
        potentialVoids_sec.push_back(i->at(1));
        potentialVoids_sec.push_back(i->at(1));
        potentialVoids.push_back(potentialVoids_sec);
      }
    }
    else
    {
      if((i->at(1) == (potentialVoids_sec.at(2) + 1))
         && (i->at(0) == potentialVoids_sec.at(0)))
      {
        potentialVoids_sec.at(2)  = i->at(1);
        *(--potentialVoids.end()) = potentialVoids_sec;
      }
      else
      {
        if(potentialVoids_sec.at(2) != potentialVoids_sec.at(1))
        {
          potentialVoids_sec.clear();
          if(i != (--availableSpecSlots.end()))
          {
            potentialVoids_sec.push_back(i->at(0));
            potentialVoids_sec.push_back(i->at(1));
            potentialVoids_sec.push_back(i->at(1));
            potentialVoids.push_back(potentialVoids_sec);
          }
        }
        else
        {
          potentialVoids_sec.clear();
          potentialVoids.erase(--potentialVoids.end());
          if(i != (--availableSpecSlots.end()))
          {
            potentialVoids_sec.push_back(i->at(0));
            potentialVoids_sec.push_back(i->at(1));
            potentialVoids_sec.push_back(i->at(1));
            potentialVoids.push_back(potentialVoids_sec);
          }
        }
      }
    }
  }

#ifdef DEBUG_print_potential_voids
  cout << "\033[0;32mPRINT\033[0m "
       << "potential voids for this time point" << endl;

  list<vector<unsigned int>>::iterator m;
  cout << "  ";
  for(m = potentialVoids.begin(); m != potentialVoids.end(); m++)
  {
    for(long unsigned int j = 0; j < m->size(); j++)
    {
      cout << m->at(j) << ' ';
    }
    cout << "     ";
  }
  cout << endl;
#endif
}

/**** Check the availability of a assignedSpectralSegment for a specific time
 * point on the lightpath ****/
void
ResourceAssignment::check_resource_availibility(
    list<vector<vector<vector<vector<bool>>>>>::iterator
                 iter, // lterator for network->spectralSlots
    vector<int> &circuitRoute, bool *tempAvailableFlag)
{
  for(long unsigned int j = 0; j < assignedSpectralSegments.size(); j++)
  {
    for(long unsigned int i = 1; i < circuitRoute.size(); i++)
    {
      for(int k = assignedSpectralSegments[j][1];
          k <= assignedSpectralSegments[j][2]; k++)
      {
        if(iter->at(circuitRoute[i - 1])
               .at(circuitRoute[i])
               .at(assignedSpectralSegments[j][0])
               .at(k)
           == true)
        {
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
    shared_ptr<CircuitRequest> circuitRequest_ptr)
{
  for(long unsigned int i = 0; i < network->candidateBR.size(); i++)
  {
    SuperChannel sC;
    sC.sCBitRate = network->candidateBR.at(i);
    candidateSCs.push_back(sC);
  }

  for(long unsigned int i = 0; i < candidateSCs.size(); i++)
  {
    modulationFormats_ptr->mf_chosen(
        circuitRoute_ptr, &(candidateSCs[i].numofMSSs),
        &(candidateSCs[i].sCBitRate), &(candidateSCs[i].mF),
        &(candidateSCs[i].mFBitsperSignal));

    if(candidateSCs[i].numofMSSs
       != 0) // 0 means no availability for this super channel option
    {
      candidateSCs[i].numofSCs4Request = ceil(
          (double)circuitRequest_ptr->bitRate / candidateSCs[i].sCBitRate);
      candidateSCs[i].numofMSSs4Request
          = (candidateSCs[i].numofMSSs + GB) * candidateSCs[i].numofSCs4Request;
    }
  }
#ifdef DEBUG_print_SC_info
  cout << "\033[0;32mPRINT\033[0m "
       << "SC related info" << endl;
  for(long unsigned int i = 0; i < candidateSCs.size(); i++)
  {
    cout << "  ";
    cout << "BR4SC: " << candidateSCs[i].sCBitRate
         << "    MF: " << candidateSCs[i].mF
         << "    SSs4SC: " << candidateSCs[i].numofMSSs << " SCs4Request "
         << candidateSCs[i].numofSCs4Request
         << "    SSs4request: " << candidateSCs[i].numofMSSs4Request << endl;
  }
#endif
}

void
ResourceAssignment::sort_super_channel(
    ModulationFormats *modulationFormats_ptr, vector<int> *circuitRoute_ptr,
    shared_ptr<CircuitRequest> circuitRequest_ptr,
    SuperChannelQueue *        superChannelQueue_ptr)
{

  super_channel_info(modulationFormats_ptr, circuitRoute_ptr,
                     circuitRequest_ptr);
  for(long unsigned int i = 0; i < candidateSCs.size(); i++)
  {
    if(candidateSCs[i].numofMSSs == 0)
      continue;
    else
      superChannelQueue_ptr->insert(&candidateSCs[i]);
  }
  bool *availableFlag_ptr;

#ifdef DEBUG_print_sorted_SC
  cout << "\033[0;32mPRINT\033[0m "
       << "Sorted Super Channels:" << endl;
  superChannelQueue_ptr->print_BR();
#endif
}

void
ResourceAssignment::match_SC_and_voids(
    unsigned int bitRate, bool *availableFlag_ptr,
    // list<vector<unsigned int>>::iterator potentialVoids_iter,
    list<SuperChannel>::iterator sCIter, unsigned int *numofGB_ptr)
{

  auto        potentialVoids_iter = potentialVoids.begin();
  vector<int> assignedSpectralSegments_sec;
  while(bitRate > 0)
  {
    /** Maybe useful when using multiple SC options for allocation **/
    // if(potentialVoids_iter == potentialVoids.end())
    // {
    //   if(i != 0)
    //     NextSC = true;
    //   else
    //     NextSC = false;
    //   break;
    // }
    if(potentialVoids_iter == potentialVoids.end())
    {
      *availableFlag_ptr = false;
      break;
    }
    else if(potentialVoids_iter->at(2) - potentialVoids_iter->at(1) + 1
            == sCIter->numofMSSs + GB)
    {
      assignedSpectralSegments_sec.push_back(potentialVoids_iter->at(0));
      assignedSpectralSegments_sec.push_back(potentialVoids_iter->at(1));
      assignedSpectralSegments_sec.push_back(potentialVoids_iter->at(2));
      /* May need a function HERE to record the used SC option when using
       * multiple SC options for allocation.
       * assignedSpectralSegments_sec.push_back(indication of the SC
       * option) as assignedSpectralSegments_sec[3]*/
      assignedSpectralSegments.push_back(assignedSpectralSegments_sec);
      assignedSpectralSegments_sec.clear();
      if(bitRate >= sCIter->sCBitRate)
        bitRate -= sCIter->sCBitRate;
      else
        bitRate = 0;
      (*numofGB_ptr)++;
      potentialVoids_iter = potentialVoids.erase(potentialVoids_iter);

#ifdef DEBUG_print_potential_voids
      cout << "\033[0;32mThe SC is \033[0m " << sCIter->sCBitRate << endl;
      list<vector<unsigned int>>::iterator probeIndex;
      cout << "\033[0;32mPRINT\033[0m "
           << "Sorted Voids" << endl;
      for(probeIndex = potentialVoids.begin();
          probeIndex != potentialVoids.end(); probeIndex++)
      {
        for(int j = 0; j < probeIndex->size(); j++)
        {
          cout << probeIndex->at(j) << ' ';
        }
        cout << "    ";
      }
      cout << endl;
#endif

      // break;
      continue;
    }
    else if(potentialVoids_iter->at(2) - potentialVoids_iter->at(1) + 1
            > sCIter->numofMSSs + GB)
    {
      assignedSpectralSegments_sec.push_back(potentialVoids_iter->at(0));
      assignedSpectralSegments_sec.push_back(potentialVoids_iter->at(1));
      assignedSpectralSegments_sec.push_back(potentialVoids_iter->at(1)
                                             + sCIter->numofMSSs + GB - 1);
      /* May need a function HERE to record the used SC option when using
       * multiple SC options for allocation.
       * assignedSpectralSegments_sec.push_back(indication of the SC
       * option) as assignedSpectralSegments_sec[3]*/
      assignedSpectralSegments.push_back(assignedSpectralSegments_sec);
      assignedSpectralSegments_sec.clear();
      if(bitRate >= sCIter->sCBitRate)
        bitRate -= sCIter->sCBitRate;
      else
        bitRate = 0;
      if(potentialVoids_iter->at(2)
             - (potentialVoids_iter->at(1) + sCIter->numofMSSs + GB) + 1
         > GB)
      {
        potentialVoids_iter->at(1)
            = potentialVoids_iter->at(1) + sCIter->numofMSSs + GB;
      }
      else
      {
        potentialVoids_iter = potentialVoids.erase(potentialVoids_iter);
      }
      (*numofGB_ptr)++;

#ifdef DEBUG_print_potential_voids
      cout << "\033[0;32mThe SC is \033[0m " << sCIter->sCBitRate << endl;
      list<vector<unsigned int>>::iterator probeIndex;
      cout << "\033[0;32mPRINT\033[0m "
           << "Sorted Segments" << endl;
      cout << "  ";
      for(probeIndex = potentialVoids.begin();
          probeIndex != potentialVoids.end(); probeIndex++)
      {
        for(int j = 0; j < probeIndex->size(); j++)
        {
          cout << probeIndex->at(j) << ' ';
        }
        cout << "    ";
      }
      cout << endl;
#endif
      // break;
      continue;
    }
    else if((potentialVoids_iter->at(2) - potentialVoids_iter->at(1) + 1)
            < sCIter->numofMSSs + GB)
    {
      potentialVoids_iter++;
    }
  }
}

void
ResourceAssignment::enlarge_time_snapshot(
    list<vector<vector<vector<vector<bool>>>>> &timeSnapshots,
    long long                                   releaseTime)
{

  if(releaseTime > network->lastMiliSec)
  {
    long long cnt = releaseTime - network->lastMiliSec - 1;
    while(cnt != 0)
    {
      network->spectralSlots.push_back(network->resourceSnapshot);
      cnt--;
    }
    network->lastMiliSec
        = releaseTime - 1; // The size of (0 to sometime - 1) is sometime.
  }
}

long long
ResourceAssignment::precision_double(double num)
{
  double eps = 0.000005;
  num += eps;
  return floor(num * 1000);
}

void
ResourceAssignment::handle_requests(
    shared_ptr<CircuitRequest> circuitRequest_ptr)
{
  RoutingTable routingTable(network);

  vector<int>  circuitRoute; // store the nodes on the path
  bool         availableFlag     = false; // global flag
  string       mF                = "BPSK";
  unsigned int bR                = 0;
  unsigned int mFBitsperSignal   = 0;
  bool         tempAvailableFlag = true; // flag just for handling duplicates
  long long    advTime;
  unsigned int numofGB; // number of SEGs

  // Find route
  circuitRoute = routingTable.get_shortest_path(circuitRequest_ptr->src,
                                                circuitRequest_ptr->dest);
  ModulationFormats modulationFormats(circuitRequest_ptr, network);

  // To append useful time snapshot
  list<vector<vector<vector<vector<bool>>>>>::iterator timeIter;
  long long startTime   = precision_double(circuitRequest_ptr->startTime);
  long long releaseTime = precision_double(circuitRequest_ptr->releaseTime);
  long long duration    = precision_double(circuitRequest_ptr->duration);

  enlarge_time_snapshot(network->spectralSlots, releaseTime);

  timeIter          = network->spectralSlots.begin();
  advTime           = startTime - network->firstMiliSec;
  advance(timeIter, advTime);

  long long durationCount = duration; // discrete duration count in ms
  long long timeCount     = startTime;

  // Stores Attributes for all periods of time snapshots for LPS
  // [number of LPSs][number of snapshots for the same time point]
  // [{start_time, end_time, core, first_SS, last_SS}]
  vector<vector<vector<long long>>> snapshotsAttributes;
  // [number of snapshots for the same time point]
  // [{start_time, end_time, core, first_SS, last_SS}]
  vector<vector<long long>>         snapshotsAttributes_sec;
  // Stores Attributes for a period of time snapshots:
  // {start_time, end_time (include in the time point), core, first spectral
  // slot, last spectral slot}
  // The period is: [start_time, end_time], does not
  // like request->startTime and request->releaseTime, [startTime, releaseTime)
  vector<long long> snapshotsAttributes_third(5, 0);

  /**** ONLY LPS mode ****/
  // Compute Modulation
  modulationFormats.mf_chosen(
      &circuitRoute, &circuitRequest_ptr->occupiedSpectralSlots,
      &circuitRequest_ptr->bitRate, &mF, &mFBitsperSignal);
  circuitRequest_ptr->occupiedSpectralSlots
      = circuitRequest_ptr->occupiedSpectralSlots + GB; // For data + GB
  bR = circuitRequest_ptr->bitRate;
  if(bR == 40)
    bR = 50;
  /*** Considered time point by time point ***/
  for(; durationCount != 0; timeIter++)
  {
    vector<int> assignedSpectralSegments_sec;

    // If mF == "Fail", no lightpath can be established
    // Does not need to consider any time point
    if(mF == "Fail")
    {
      availableFlag = false;
      break;
    }

    /** Handle duplicates **/
    tempAvailableFlag = true;
    if(!assignedSpectralSegments.empty())
    {

      check_resource_availibility(timeIter, circuitRoute, &tempAvailableFlag);
      if(tempAvailableFlag == true)
      {
        snapshotsAttributes_third[1] = timeCount;
        if(durationCount == 1)
        {
          snapshotsAttributes_sec.clear();
          snapshotsAttributes_sec.push_back(snapshotsAttributes_third);
          snapshotsAttributes.push_back(snapshotsAttributes_sec);
        }
        timeCount++;
        durationCount--;
        continue;
      }
      else
      {
        snapshotsAttributes_sec.clear();
        snapshotsAttributes_sec.push_back(snapshotsAttributes_third);
        snapshotsAttributes.push_back(snapshotsAttributes_sec);
      }
    }

    // Initialize snapshotAttributes_sec with all 0s
    fill(snapshotsAttributes_third.begin(), snapshotsAttributes_third.end(), 0);
    snapshotsAttributes_third[0] = timeCount;

    /** Search for all potential voids for this time point **/
    potentialVoids.clear();
    create_voids(&circuitRoute, timeIter);

    /** Pre-allocation **/
    if(potentialVoids.empty())
      availableFlag = false;
    else
    {
      list<vector<unsigned int>>::iterator i; // iterator for potentialVoids

      availableFlag = false;
      for(i = potentialVoids.begin(); i != potentialVoids.end(); i++)
      {
        // If the voids is not smaller than that occupiedSpectralSlots
        if((i->at(2) - i->at(1) + 1)
           >= (circuitRequest_ptr->occupiedSpectralSlots))
        {
          for(long unsigned int w = 0; w < (i->size() - 1); w++)
          {
            snapshotsAttributes_third[w + 2] = i->at(w);
          }
          snapshotsAttributes_third[4]
              = i->at(1) + circuitRequest_ptr->occupiedSpectralSlots - 1;
          for(long unsigned int m = 2; m < snapshotsAttributes_third.size();
              m++)
            assignedSpectralSegments_sec.push_back(
                snapshotsAttributes_third.at(m));
          assignedSpectralSegments.clear();
          assignedSpectralSegments.push_back(assignedSpectralSegments_sec);
          availableFlag = true;
          break;
        }
      }
    }
    if(availableFlag == false)
      break;

#ifdef DEBUG_print_availableSpectralSegments
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
         << endl;
    cout << endl;
#endif

    snapshotsAttributes_third[1] = timeCount;
    if(durationCount == 1)
    {
      snapshotsAttributes_sec.clear();
      snapshotsAttributes_sec.push_back(snapshotsAttributes_third);
      snapshotsAttributes.push_back(snapshotsAttributes_sec);
    }
    timeCount++;
    durationCount--;
  }

  // Check if times of LP exceeds the limitation (MAX_NUMOFTIMESLICES)
  if(snapshotsAttributes.size() > MAX_NUMOFTIMESLICES)
    availableFlag = false;
  /**** End ONLY LPS mode ****/

  /**** SEG-LPS mode ****/
#ifndef LALA
  if(availableFlag == false)
  {
    // Reset timeIter
    timeIter = network->spectralSlots.begin();
    advTime  = startTime - network->firstMiliSec;
    advance(timeIter, advTime);


    // Process super channel options
    SuperChannelQueue superChannelQueue;
    candidateSCs.clear();
    sort_super_channel(&modulationFormats, &circuitRoute, circuitRequest_ptr,
                       &superChannelQueue);

    // Reset assignedSpectralSegments
    assignedSpectralSegments.clear();

    durationCount           = duration; // discrete duration count in ms
    timeCount               = startTime;
    // Delete the super channel option that is used in ONLY LPS mode
    auto iter = superChannelQueue.begin();
    for(; iter != superChannelQueue.end(); iter++)
    {
      SuperChannel *sC_ptr = superChannelQueue.get(iter);
      unsigned int  sCBitRate = circuitRequest_ptr->bitRate;
      if(sCBitRate == 40)
        sCBitRate = 50;
      if(sCBitRate == sC_ptr->sCBitRate)
      {
        iter = superChannelQueue.erase(iter);
        iter--; // This iter-- is used to neutralize iter++
      }
    }
#ifdef DEBUG_print_sorted_SC
    cout << "\033[0;32mPRINT\033[0m "
         << "Sorted Super Channels (delete LPS SC):" << endl;
    superChannelQueue.print_BR();
#endif

    /*** Considered time point by time point ***/
    // The logic of SEG based on the truth that availableFlag is true
    availableFlag = true;
    for(; durationCount != 0; timeIter++)
    {
      /** Handle duplicates **/
      tempAvailableFlag = true;
      if(!assignedSpectralSegments.empty())
      {
        check_resource_availibility(timeIter, circuitRoute, &tempAvailableFlag);
        if(tempAvailableFlag == true)
        {
          for(int i = 0; i < snapshotsAttributes_sec.size(); i++)
          {
            snapshotsAttributes_sec[i][1] = timeCount;
          }


          if(durationCount == 1)
          {
            snapshotsAttributes.push_back(snapshotsAttributes_sec);
          }
          timeCount++;
          durationCount--;
          continue;
        }
        else
        {
          snapshotsAttributes.push_back(snapshotsAttributes_sec);
        }
      }

      // ???
      vector<int> assignedSpectralSegments_sec;
      auto snapshotStartTime = timeCount;

      // Reset assignedSpectralSegments
      assignedSpectralSegments.clear();

      // Find all the voids for this time point
      potentialVoids.clear();
      create_voids(&circuitRoute, timeIter);
      potentialVoids_backup = potentialVoids;

      /** ALLOCATION EFFICIENCY DRIVEN mode **/
      if(potentialVoids.empty())
      {
        availableFlag = false;
        break;
      }
      else
      {
        list<vector<unsigned int>>::iterator potentialVoids_iter
            = potentialVoids.begin();
        unsigned int bitRate = circuitRequest_ptr->bitRate;
        // The iterator that points to the chosen SC in superChannelQueue
        auto sCIter = superChannelQueue.begin();
        unsigned int sCIter_cnt = 0;
        match_SC_and_voids(bitRate, &availableFlag, sCIter, &numofGB);

        if(availableFlag == true)
        {
          mF = sCIter->mF;
          bR = sCIter->sCBitRate;
          snapshotsAttributes_sec.clear();
          for(int i = 0; i < assignedSpectralSegments.size(); i++)
          {
            fill(snapshotsAttributes_third.begin(),
                 snapshotsAttributes_third.end(), 0);
            snapshotsAttributes_third[0] = snapshotStartTime;
            for(int j = 0; j < assignedSpectralSegments[i].size(); j++)
            {

              snapshotsAttributes_third[j + 2] = assignedSpectralSegments[i][j];

            }
            snapshotsAttributes_sec.push_back(snapshotsAttributes_third);
          }
        }
        // ???
        // // ALLOCATION EFFICIENCY mode use the smallest allocation size SCs
        // if(availableFlag == false)
        //   break;
      }

      /** NUMBER OF SEGMENTS DRIVEN mode **/
      if(assignedSpectralSegments.size() > network->maxAllowedSegments)
      {

        // Reset the potentialVoids
        potentialVoids = potentialVoids_backup;

        // Delete the used super channel option in ALLOCATION EFFICIENCY
        // DRIVEN mode
        auto iter = superChannelQueue.begin();
        for(; iter != superChannelQueue.end(); iter++)
        {
          SuperChannel *sC_ptr = superChannelQueue.get(iter);
          if(sC_ptr->numofSCs4Request > network->maxAllowedSegments)
          {
            iter = superChannelQueue.erase(iter);
            iter--; // This iter-- is used to neutralize iter++
          }
        }
#ifdef DEBUG_print_sorted_SC
        cout
            << "\033[0;32mPRINT\033[0m "
            << "Sorted Super Channels (delete SCs that exceeds SEG limitation):"
            << endl;
        superChannelQueue.print_BR();
#endif

        assignedSpectralSegments.clear();
        iter         = superChannelQueue.begin();
        if(superChannelQueue.empty())
          availableFlag = false;
        else
        {
          auto MSSs4SC = superChannelQueue.get(iter)->numofMSSs;
          for(iter = superChannelQueue.begin(); iter != superChannelQueue.end();
              iter++)
          {
            if(superChannelQueue.get(iter)->numofMSSs > MSSs4SC)
              continue;
            auto sCIter          = superChannelQueue.begin();
            numofGB              = 0;
            unsigned int bitRate = circuitRequest_ptr->bitRate;
            list<vector<unsigned int>>::iterator potentialVoids_iter
                = potentialVoids.begin();

            match_SC_and_voids(bitRate, &availableFlag, sCIter, &numofGB);

            if(availableFlag == true)
            {
              mF = iter->mF;
              bR = iter->sCBitRate;
              snapshotsAttributes_sec.clear();
              for(int i = 0; i < assignedSpectralSegments.size(); i++)
              {
                fill(snapshotsAttributes_third.begin(),
                     snapshotsAttributes_third.end(), 0);
                snapshotsAttributes_third[0] = snapshotStartTime;
                for(int j = 0; j < assignedSpectralSegments[i].size(); j++)
                {

                  snapshotsAttributes_third[j + 2]
                      = assignedSpectralSegments[i][j];

                }
                snapshotsAttributes_sec.push_back(snapshotsAttributes_third);
              }
            }
          }
        }
      }
      if(availableFlag == false)
        break;

      for(int i = 0; i < snapshotsAttributes_sec.size(); i++)
      {
        snapshotsAttributes_sec[i][1] = timeCount;
      }
      if(durationCount == 1)
      {
        snapshotsAttributes.push_back(snapshotsAttributes_sec);
      }
      timeCount++;
      durationCount--;
    }
  }
  if(snapshotsAttributes.size() > MAX_NUMOFTIMESLICES)
    availableFlag = false;
#endif
  /**** End SEG-LPS mode ****/

  /**** Real allocation and printout ****/
  if(availableFlag == false)
  {
    network->numofDoneRequests++;

#ifdef DEBUG_collect_eventID_of_blocked_requests
    network->blockedRequests.push_back(circuitRequest_ptr->eventID);
#endif

#ifdef PRINT_allocation_block_release
    cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
         << endl;
    cout << "Blocked Request ID: " << circuitRequest_ptr->eventID << endl;
    cout << "Modulation Format: " << mF << endl;
    cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
         << endl;
#endif
    assignedSpectralSegments.clear();

    network->numofFailedRequests++;
    switch(circuitRequest_ptr->bitRate)
    {
      case 40:
        network->block_40++;
        break;
      case 100:
        network->block_100++;
        break;
      case 400:
        network->block_400++;
        break;
    }
  }
  else if(availableFlag == true)
  {
    timeIter = network->spectralSlots.begin();

    // Occupy SSs
    for(long unsigned int i = 0; i < snapshotsAttributes.size(); i++)
    {
      startTime = snapshotsAttributes[i][0][0];
      timeIter = network->spectralSlots.begin();
      advTime  = startTime - network->firstMiliSec;
      advance(timeIter, advTime);
      durationCount
          = snapshotsAttributes[i][0][1] - snapshotsAttributes[i][0][0] + 1;

      for(long unsigned int j = 0; j < snapshotsAttributes[i].size(); j++)
      {
        for(; durationCount != 0; timeIter++)
        {
          for(long unsigned int r = 1; r < circuitRoute.size(); r++)
          {
            for(int k = snapshotsAttributes[i][j][3];
                k < snapshotsAttributes[i][j][4] + 1; k++)
            {
              timeIter->at(circuitRoute[r - 1])
                  .at(circuitRoute[r])
                  .at(snapshotsAttributes[i][j][2])
                  .at(k)
                  = true;
            }
          }
          durationCount--;
        }
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
         << "  Destination: " << circuitRequest_ptr->dest
         << "  ModulationFormats: " << mF << endl;
    cout << "Path: ";
    for(unsigned int t = 0; t < circuitRoute.size() - 1; t++)
      cout << circuitRoute.at(t) << " --> ";
    cout << circuitRoute.at(circuitRoute.size() - 1) << endl;
    cout << "Super Channel Option: " << bR << endl;
    ;
    for(long unsigned int i = 0; i < snapshotsAttributes.size(); i++)
    {
      cout << "From " << snapshotsAttributes[i][0][0] << " to "
           << snapshotsAttributes[i][0][1] << endl;
      for(long unsigned int j = 0; j < snapshotsAttributes[i].size(); j++)
      {
        cout << "  ";
        cout << "Core: " << snapshotsAttributes[i][j][2]
             << "  Spectral Segments: " << snapshotsAttributes[i][j][3]
             << " to " << snapshotsAttributes[i][j][4] << endl;
      }
    }
    cout << "# of Segments: " << snapshotsAttributes[0].size() << endl;
    cout << "# of Time-Slices: " << snapshotsAttributes.size() << endl;
    // cout << "# of Cores Used: " << '1' << endl;
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
        snapshotsAttributes[0].size());
    eventQueue->queue_insert(event_ptr);

    // circuitRelease = new CircuitRelease(
    //     circuitRequest->EventID, CircuitRoute, assignedSpectralSegments,
    //     circuitRequest->ArrivalTime, circuitRequest->StartTime,
    //     circuitRequest->StartTime + circuitRequest->Duration,
    //     circuitRequest->StartTime + circuitRequest->Duration, 1);
    // eventQueue->queue_insert(circuitRelease);

    if(bR == 25)
      network->numof25SC += snapshotsAttributes[0].size();
    else if(bR == 50)
    {
      if(mF == "QPSK")
        network->numof50SC2 += snapshotsAttributes[0].size();
      else if(mF == "16QAM")
        network->numof50SC4 += snapshotsAttributes[0].size();
      else if(mF == "64QAM")
        network->numof50SC6 += snapshotsAttributes[0].size();
    }
    else if(bR == 100)
    {
      if(mF == "QPSK")
        network->numof100SC2 += snapshotsAttributes[0].size();
      else if(mF == "16QAM")
        network->numof100SC4 += snapshotsAttributes[0].size();
      else if(mF == "64QAM")
        network->numof100SC6 += snapshotsAttributes[0].size();
    }
    else if(bR == 200)
    {
      if(mF == "QPSK")
        network->numof200SC2 += snapshotsAttributes[0].size();
      else if(mF == "16QAM")
        network->numof200SC4 += snapshotsAttributes[0].size();
      else if(mF == "64QAM")
        network->numof200SC6 += snapshotsAttributes[0].size();
    }
    else if(bR == 400)
    {
      if(mF == "QPSK")
        network->numof400SC2 += snapshotsAttributes[0].size();
      else if(mF == "16QAM")
        network->numof400SC4 += snapshotsAttributes[0].size();
      else if(mF == "64QAM")
        network->numof400SC6 += snapshotsAttributes[0].size();
    }

    network->numofAllocatedRequests++;
    network->numofTransponders += snapshotsAttributes[0].size();
    // network->numofSSs4Data
    //     = (snapshotsAttributes[0][0][4] - snapshotsAttributes[0][0][3] + 1 -
    //     GB)
    //       * snapshotsAttributes[0].size();
    network->totalTranspondersUsed += snapshotsAttributes[0].size();
    network->totalHoldingTime += circuitRequest_ptr->duration;
    // network->totalCoresUsed++;
    network->totalLPS += snapshotsAttributes.size();
    network->totalSSs4Data += (snapshotsAttributes[0][0][4]
                               - snapshotsAttributes[0][0][3] + 1 - GB)
                              * snapshotsAttributes[0].size();
    network->totalMDataSize += network->totalSSsOccupied
        += (snapshotsAttributes[0][0][4] - snapshotsAttributes[0][0][3] + 1)
           * snapshotsAttributes[0].size();
  }
}

void
ResourceAssignment::handle_releases(
    shared_ptr<CircuitRelease> circuitRelease_ptr)
{

  network->numofDoneRequests++;
  network->numofTransponders -= circuitRelease_ptr->transpondersUsed;

#ifdef PRINT_allocation_block_release
  cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
       << endl;
  cout << "Release Event ID: " << circuitRelease_ptr->eventID
       << "\tTime: " << circuitRelease_ptr->eventTime << endl;
  cout << "Number of transpondersUsed " << network->numofTransponders << endl;
  cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
       << endl;
#endif
}
