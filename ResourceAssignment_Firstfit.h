#ifndef _RESOURCEASSIGNMENT_H
#define _RESOURCEASSIGNMENT_H

#include "Event.h"
#include "EventQueue.h"
#include "ModulationFormats.h"
#include "Network.h"
#include "RoutingTable.h"
#include <cmath>

// Light-segment without time info
class SpectrumSegment {
public:
  SpectrumSegment() {}
  ~SpectrumSegment() {}
  unsigned int core;    // The core for this spectrum segment
  unsigned int firstSS; // The first spectral slot on the core above for this
                        // specturm segment
  unsigned int lastSS;  // The last spectral slot on the core above for this
                        // specturm segment
  string       mF;      // The mF used for this spectrum segment
  unsigned int bitRate; // The bitRate for this spectrum segment
};

// Light-segment Attributes
class LightSegment : public SpectrumSegment {
public:
  LightSegment() {}
  ~LightSegment() {}

  // Each period is: [startTimeSlot, endTimeSlot], including the endTimeSlot.
  long long    startTimeSlot; // The start time slot of this period
  long long    endTimeSlot;   // The end time slot of this period

  /* unsigned int core;          // The core for this light-segment */
  /* unsigned int firstSS; // The first spectral slot on the core above for this
   */
  /*                       // light-segment */
  /* unsigned int */
  /*     lastSS; // The last spectral slot on the core above for this
   * light-segment */
  /* string       mF;      // The mF used for this light-segment */
  /* unsigned int bitRate; // The bitRate for this light-segment */
};

class SuperChannel {
public:
  SuperChannel() {};
  ~SuperChannel() {};

  /*** Variables ***/
  unsigned int sCBitRate; // Bit rate for this SC
  unsigned int numMSSs; // The number of SSs for after modulation for this SC
  string       mF;        // Modulation format for this SC
  unsigned int
      mFBitsperSignal; // The associated bits per signal for the MF above
  unsigned int
      numSCs4Request; // The needed number of SCs for the given request
  unsigned int numMSSs4Request; // The needed number of SSs for the given
                                  // request (data + GB)
};

class SuperChannelQueue {
public:
  SuperChannelQueue() {}
  ~SuperChannelQueue() {}

  void
  insert(SuperChannel *sC_ptr) {
    list<SuperChannel>::iterator sCList_iter;

    if(sCList.empty())
      sCList.push_back(*sC_ptr);
    else {
      // Move the iter to the last element for sCList
      sCList_iter = sCList.end();
      sCList_iter--;

      bool insertFlag = false;
      for(; sCList_iter != sCList.begin(); sCList_iter--) {
        if(sCList_iter->numMSSs == 0)
          continue;
        if(sC_ptr->numMSSs4Request >= sCList_iter->numMSSs4Request) {
          sCList_iter++;
          sCList.insert(sCList_iter, *sC_ptr);
          insertFlag = true;
          break;
        }
      }
      // consider when sCList_iter == sCList.begin()
      if(insertFlag == false) {
        if(sC_ptr->numMSSs4Request >= sCList_iter->numMSSs4Request) {
          sCList_iter++;
          sCList.insert(sCList_iter, *sC_ptr);
        }
        else if(sC_ptr->numMSSs4Request < sCList_iter->numMSSs4Request)
          sCList.push_front(*sC_ptr);
      }
    }
  }
  void
  insert_by_SCsize() {}
  void
  print_BR() {
    list<SuperChannel>::iterator iter = sCList.begin();
    cout << "  ";
    for(; iter != sCList.end(); iter++)
      cout << iter->sCBitRate << ' ';
    cout << endl;
  }
  SuperChannel *
  get(list<SuperChannel>::iterator iter) {
    return &(*iter);
  }
  list<SuperChannel>::iterator
  begin() {
    return sCList.begin();
  }
  list<SuperChannel>::iterator
  end() {
    return sCList.end();
  }
  list<SuperChannel>::iterator
  erase(list<SuperChannel>::iterator iter) {
    iter = sCList.erase(iter);
    return iter;
  }
  bool
  empty() {
    bool ret;
    if(sCList.empty())
      ret = true;
    else
      ret = false;
    return ret;
  }

private:
  list<SuperChannel> sCList;
};

class PotentialVoid {
public:
  PotentialVoid() {
    core    = 100000;
    startSS = 100000;
    endSS   = 100000;
  }
  ~PotentialVoid() {}

  size_t core;
  size_t startSS; // the first spectral slot index for the void
  size_t endSS;   // the last spectral slot index for the void

  void
  clear() {
    core    = 100000;
    startSS = 100000;
    endSS   = 100000;
  }

  bool
  empty() {
    if(core == 100000)
      return true;
    else
      return false;
  }
};


class ResourceAssignment {
public:
  ResourceAssignment(Network *net, shared_ptr<EventQueue> eq) {
    network    = net;
    eventQueue = eq;
  }
  ~ResourceAssignment() {}

  /*** Variables ***/
  // The list of candidate super channels
  vector<SuperChannel> candidateSCs;
  // Stores all the potential voids for allocation
  // [number of voids][{core, first spectrum slot, last spectrum slot}]
  vector<vector<int>> availableSpectralSegments;
  // Stores the assigned spectral chunk for a specific time slot
  // [core, first spectral slot, last spectral slot, super channel bit rate]
  list<SpectrumSegment> assignedSpectralSegments;
  // Stores the available spectral slots for this time point
  list<vector<unsigned int>> availableSpecSlots;
  // Stores the potential voids that are formed by the available spectral slots
  // for this time point
  list<PotentialVoid>           potentialVoids;
  list<PotentialVoid>           potentialVoids_backup;
  map<int, list<PotentialVoid>> potentialVoidsIso;
  // Backup potentialVoidsIso
  map<int, list<PotentialVoid>> potentialVoidsIso_backup;

  /*** Functions ***/
  void
  handle_requests(shared_ptr<CircuitRequest> circuitRequest_ptr);
  void
  handle_releases(shared_ptr<CircuitRelease> circuitRelease_ptr);
  void
  enlarge_time_snapshot(
      list<vector<vector<vector<vector<bool>>>>> &timeSnapshots,
      long long                                   releaseTime);

  // Check the available spectral slots on the first link
  void
  check_availability_first_link(
      unsigned int predecessor, unsigned int successor,
      list<vector<vector<vector<vector<bool>>>>>::iterator timeIter);
  // Check the available spectral slots on other than the first link
  void
  check_availability_following_links(
      vector<int> *                                        circuitRoute_ptr,
      list<vector<vector<vector<vector<bool>>>>>::iterator timeIter);
  // Form the potential voids with the available spectral slots for this time
  // point
  void
  create_voids(vector<int> *circuitRoute_ptr,
               list<vector<vector<vector<vector<bool>>>>>::iterator timeIter);
  // Check the availability of a assignedSpectralSegment for a specific time
  // point on the lightpath
  void
  check_resource_availibility(
      list<vector<vector<vector<vector<bool>>>>>::iterator iter,
      vector<int> &circuitRoute, bool *tempAvailableFlag);

  void
  super_channel_info(ModulationFormats *        modulationFormats_ptr,
                     vector<int> *              circuitRoute_ptr,
                     shared_ptr<CircuitRequest> circuitRequest_ptr);

  void
  sort_super_channel(ModulationFormats *        modulationFormats_ptr,
                     vector<int> *              circuitRoute_ptr,
                     shared_ptr<CircuitRequest> circuitRequest_ptr,
                     SuperChannelQueue *        superChannelQueue);
  void
  isolate_potentialVoids();
  int
  match_SC_and_voids(unsigned int bitRate, bool *availableFlag_ptr,
                     list<SuperChannel>::iterator sCIter,
                     unsigned int                *NumofGB_ptr,
                     list<PotentialVoid>         &potentialVoid_ThisCore);

private:
  Network *              network;
  shared_ptr<EventQueue> eventQueue;
};

#endif
