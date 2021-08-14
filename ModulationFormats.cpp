#include "ModulationFormats.h"
#include <cmath>
#include <iostream>
#include <string>

using namespace std;

/****************************************
 * Options for Super Channels: 25 50 100 200 400
 * Options for Modulation Formats:
 *     QPSK, 16QAM, 64QAM
 ****************************************/

unsigned int
ModulationFormats::spectralslots_computation(unsigned int bitsPerSymbol,
                                             unsigned int bitRate)
{
  unsigned int SpectralSlots;
  SpectralSlots = ceil((double)bitRate / bitsPerSymbol / 12.5);
  return SpectralSlots;
}

double
ModulationFormats::search_link_weight(unsigned int predecessor,
                                      unsigned int successor)
{
  return network->nodesWeight[predecessor][successor];
}

void
ModulationFormats::mf_chosen(vector<int> * shortestPath,
                             unsigned int *occupiedSpectralSlots,
                             unsigned int *bitRate, string *mF,
                             unsigned int *mFBitsperSignal)
{
  double       totalDist = 0;    // distance of the lightpath
  double       dist      = 0;    // distance between two ajacent nodes
  unsigned int spectralSlots_am; // SpectralSlots after modulation

  /*** Compute the total distance from source to destination ***/
  for(long unsigned int i = 1; i < shortestPath->size(); i++)
  {
    dist = search_link_weight(shortestPath->at(i - 1), shortestPath->at(i));
    totalDist = totalDist + dist;
  }

  auto sC = *bitRate;
  switch(sC)
  {
    case 25:
      if(totalDist <= 22160)
      {
        this->mF         = QPSK;
        *mFBitsperSignal = 2;
        spectralSlots_am = spectralslots_computation(*mFBitsperSignal, 25);
        *mF              = "QPSK";
      }
      else
      {
        this->mF         = Failure;
        *mFBitsperSignal = 0;
        *mF              = "Fail";
        spectralSlots_am = 0;
      }
      break;
    // case 50:
    case 40:
      if(totalDist > 4750 && totalDist <= 11080)
      {
        this->mF         = QPSK;
        *mFBitsperSignal = 2;
        spectralSlots_am = spectralslots_computation(*mFBitsperSignal, 40);
        *mF              = "QPSK";
      }
      else if(totalDist > 1832 && totalDist <= 4750)
      {
        this->mF         = QAM16;
        *mFBitsperSignal = 4;
        *mF              = "16QAM";
        spectralSlots_am = spectralslots_computation(*mFBitsperSignal, 40);
      }
      else if(totalDist <= 1832)
      {
        this->mF         = QAM64;
        *mFBitsperSignal = 6;
        *mF              = "64QAM";
        spectralSlots_am = spectralslots_computation(*mFBitsperSignal, 40);
      }
      else
      {
        this->mF         = Failure;
        *mFBitsperSignal = 0;
        *mF              = "Fail";
        spectralSlots_am = 0;
      }
      break;
    case 50:
      if(totalDist > 4750 && totalDist <= 11080)
      {
        this->mF         = QPSK;
        *mFBitsperSignal = 2;
        spectralSlots_am = spectralslots_computation(*mFBitsperSignal, 50);
        *mF              = "QPSK";
      }
      else if(totalDist > 1832 && totalDist <= 4750)
      {
        this->mF         = QAM16;
        *mFBitsperSignal = 4;
        *mF              = "16QAM";
        spectralSlots_am = spectralslots_computation(*mFBitsperSignal, 50);
      }
      else if(totalDist <= 1832)
      {
        this->mF         = QAM64;
        *mFBitsperSignal = 6;
        *mF              = "64QAM";
        spectralSlots_am = spectralslots_computation(*mFBitsperSignal, 50);
      }
      else
      {
        this->mF         = Failure;
        *mFBitsperSignal = 0;
        *mF              = "Fail";
        spectralSlots_am = 0;
      }
      break;
    // case 75:
    // 	if (TotalDist > 3166 && TotalDist <= 7387) {
    // 		m_Format = QPSK;
    // 		*mfTimes = 2;
    // 		am_SpectralSlots = spectralslots_computation (*mfTimes, 75);
    // 		*MF = "QPSK";
    // 	}
    // 	else if (TotalDist > 1221 && TotalDist <= 3166) {
    // 		m_Format = QAM16;
    // 		*mfTimes = 4a
    // 		*MF = "16QAM";
    // 		am_SpectralSlots = spectralslots_computation (*mfTimes, 75);
    // 	}
    // 	else if (TotalDist <= 1221) {
    // 		m_Format = QAM64;
    // 		*mfTimes = 6;
    // 		*MF = "64QAM";
    // 		am_SpectralSlots = spectralslots_computation (*mfTimes, 75);
    // 	}
    // break;
    case 100:
      if(totalDist > 2375 && totalDist <= 5540)
      {
        this->mF         = QPSK;
        *mFBitsperSignal = 2;
        spectralSlots_am = spectralslots_computation(*mFBitsperSignal, 100);
        *mF              = "QPSK";
      }
      else if(totalDist > 916 && totalDist <= 2375)
      {
        this->mF         = QAM16;
        *mFBitsperSignal = 4;
        *mF              = "16QAM";
        spectralSlots_am = spectralslots_computation(*mFBitsperSignal, 100);
      }
      else if(totalDist <= 916)
      {
        this->mF         = QAM64;
        *mFBitsperSignal = 6;
        *mF              = "64QAM";
        spectralSlots_am = spectralslots_computation(*mFBitsperSignal, 100);
      }
      else
      {
        this->mF         = Failure;
        *mFBitsperSignal = 0;
        *mF              = "Fail";
        spectralSlots_am = 0;
      }
      break;
    case 200:
      if(totalDist > 1187 && totalDist <= 2770)
      {
        this->mF         = QPSK;
        *mFBitsperSignal = 2;
        spectralSlots_am = spectralslots_computation(*mFBitsperSignal, 200);
        *mF              = "QPSK";
      }
      else if(totalDist > 458 && totalDist <= 1187)
      {
        this->mF         = QAM16;
        *mFBitsperSignal = 4;
        *mF              = "16QAM";
        spectralSlots_am = spectralslots_computation(*mFBitsperSignal, 200);
      }
      else if(totalDist <= 458)
      {
        this->mF         = QAM64;
        *mFBitsperSignal = 6;
        *mF              = "64QAM";
        spectralSlots_am = spectralslots_computation(*mFBitsperSignal, 200);
      }
      else
      {
        this->mF         = Failure;
        *mFBitsperSignal = 0;
        *mF              = "Fail";
        spectralSlots_am = 0;
      }
      break;
    case 400:
      if(totalDist > 594 && totalDist <= 1385)
      {
        this->mF         = QPSK;
        *mFBitsperSignal = 2;
        spectralSlots_am = spectralslots_computation(*mFBitsperSignal, 400);
        *mF              = "QPSK";
      }
      else if(totalDist > 229 && totalDist <= 594)
      {
        this->mF         = QAM16;
        *mFBitsperSignal = 4;
        *mF              = "16QAM";
        spectralSlots_am = spectralslots_computation(*mFBitsperSignal, 400);
      }
      else if(totalDist <= 229)
      {
        this->mF         = QAM64;
        *mFBitsperSignal = 6;
        *mF              = "64QAM";
        spectralSlots_am = spectralslots_computation(*mFBitsperSignal, 400);
      }
      else
      {
        this->mF         = Failure;
        *mFBitsperSignal = 0;
        *mF              = "Fail";
        spectralSlots_am = 0;
      }
  }

  *occupiedSpectralSlots = spectralSlots_am;
}
