#include "Event.h"

CircuitRequest::CircuitRequest(unsigned int src, unsigned int dest,
                               double arrivalTime, double bookAheadTime,
                               double duration, unsigned int bitRate,
                               unsigned int occupiedSpectralSlots,
                               long long eventID, Request_t requestType)
{
  this->src                   = src;
  this->dest                  = dest;
  this->arrivalTime           = arrivalTime;
  this->bookAheadTime         = bookAheadTime;
  this->startTime             = bookAheadTime + arrivalTime;
  this->duration              = duration;
  this->releaseTime           = startTime + duration;
  this->eventTime             = arrivalTime;
  this->eventID               = eventID;
  this->eventType             = c_Request;
  this->requestType           = requestType;
  this->occupiedSpectralSlots = occupiedSpectralSlots;
  this->bitRate               = bitRate;
}

// CircuitRelease::CircuitRelease(long long eventID, vector<int> &circuitRoute,
//                                // vector<vector<int>>
//                                &occupiedSpectralSection, double arrivalTime,
//                                double startTime, double duration, double
//                                releaseTime, unsigned int transpondersUsed)
CircuitRelease::CircuitRelease(long long eventID, vector<int> &circuitRoute,
                               double arrivalTime, double startTime,
                               double duration, double releaseTime,
                               unsigned int transpondersUsed)
{
  this->circuitRoute            = circuitRoute;
  this->arrivalTime             = arrivalTime;
  this->startTime               = startTime;
  this->duration                = duration;
  // this->occupiedSpectralSection = occupiedSpectralSection;
  this->releaseTime             = releaseTime;
  this->eventTime               = releaseTime;
  this->eventType               = c_Release;
  this->eventID                 = eventID;
  this->transpondersUsed        = transpondersUsed;
}
