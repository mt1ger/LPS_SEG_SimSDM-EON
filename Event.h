#ifndef _EVENT_H
#define _EVENT_H

#include <iostream>
#include <vector>

using namespace std;

// c_ means circuit request
enum Event_t
{
  c_Request,
  c_Release
};
enum Request_t
{
  c_AR,
  c_IR
};

class Event {
public:
  Event() {}
  ~Event() {}

  // unsigned int EventID;
  double    eventTime;
  long long eventID;
  Event_t   eventType;

private:
};

class CircuitRequest : public Event {
public:
  CircuitRequest(unsigned int src, unsigned int dest, double arrivalTime,
                 double bookAheadTime, double duration, unsigned int bitRate,
                 unsigned int occupiedSpectralSlots, long long eventID,
                 Request_t requestType);
  ~CircuitRequest() {}
  unsigned int src;
  unsigned int dest;
  double       arrivalTime;
  double       bookAheadTime;
  double       startTime;
  double       duration;
  double       releaseTime;
  unsigned int bitRate;
  unsigned int occupiedSpectralSlots; // Only stores head slot and tail slot
  Request_t    requestType;
};

class CircuitRelease : public Event {
public:
  // CircuitRelease(long long eventID, vector<int> &circuitRoute,
  //                // vector<vector<int>> &occupiedSpectralSection,
  //                double arrivalTime, double startTime, double duration,
  //                double releaseTime, unsigned int transpondersUsed);
  CircuitRelease(long long eventID, vector<int> &circuitRoute,
                 double arrivalTime, double startTime, double duration,
                 double releaseTime, unsigned int transpondersUsed);
  ~CircuitRelease() {}
  vector<int> circuitRoute;
  // vector<vector<int>>
  //              occupiedSpectralSection; // Only stores head slot and tail
  //              slot
  double       arrivalTime;
  double       startTime;
  double       duration;
  double       releaseTime;
  unsigned int transpondersUsed;
};

#endif
