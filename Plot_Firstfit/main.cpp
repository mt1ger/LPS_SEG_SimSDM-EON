#include "txtParse.h"
#include <iostream>
#include <string>
#define MU 0.2

using namespace std;

int
main()
{
  vector<int> Core, Lambda, Seed, Erlang;
  int         Number = 40;

  Core.push_back(1);
  // Core.push_back(2);
  Core.push_back(4);
  // Core.push_back(7);

  for(int i = 2; i < 101; i += 5)
  {
    Lambda.push_back(i);
  }
  for(int i = 0; i < Lambda.size(); i += 1)
  {
    Erlang.push_back(Lambda[i] / MU);
  }

  Seed.push_back(26851);
  Seed.push_back(16590);
  Seed.push_back(2893);

  string parentPath = "/Users/mt1ger/Code_Projects/Simulator/DATA/"
                      "LPS_SEG_SimSDM-EON/Plot_Firstfit/";
  txtParse       tp(Core, Erlang, Seed, parentPath);
  vector<string> filePaths;
  tp.parse();
}
