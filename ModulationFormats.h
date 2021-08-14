#ifndef _MODULATIONFORMAT_H
#define _MODULATIONFORMAT_H

#include "Event.h"
#include "Network.h"

enum MF_t
{
  QPSK,
  QAM16,
  QAM64,
  Failure
};

class ModulationFormats
{
public:
  ModulationFormats(shared_ptr<CircuitRequest> cr, Network *net)
  {
    request = cr;
    network = net;
  }
  ~ModulationFormats() {}

  void
  mf_chosen(vector<int> *shortestPath, unsigned int *occupiedSpectralSlots,
            unsigned int *bitRate, string *mF, unsigned int *mFBitsperSignal);

  MF_t mF;

private:
  unsigned int
  spectralslots_computation(unsigned int bitsPerSymbol,
                            unsigned int bm_SpectralSlots);
  unsigned int
  datasize_computation(unsigned int bitsPerSymbol, unsigned int bm_DataSize);
  double
  search_link_weight(unsigned int predecessor, unsigned int successor);
  shared_ptr<CircuitRequest> request;
  Network *       network;
};

#endif
