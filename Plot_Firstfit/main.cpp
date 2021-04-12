#include "txtParse.h"
#include <iostream>
#include <string>
#define MU 0.2
#define NUMOFSEEDS 3

using namespace std;

int
main()
{
  vector<int> Core, Lambda, Seed, Erlang;
  int         Number = 40;

  // Core.push_back(1);
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

  string parentPath = "/Users/mt1ger/Code_Projects/Simulator/"
                      "LPS_SEG_SimSDM-EON/Plot_Firstfit/test0/";
  txtParse       tp(Core, Erlang, Seed, parentPath);
  vector<string> filePaths;
  // for(int i = 1; i <= 8; i *= 2)
  // {
  //   for(int j = 1; j <= 4; j *= 4)
  //   {
  //     string      filePath = parentPath;
  //     vector<int> tempCore;
  //     tempCore.push_back(j);
  //     filePath += to_string(i) + "/Plot" + to_string(j) + '/';
  //     cout << filePath << endl;
  //     tp.parse();
  //   }
  // }
  tp.parse();
}
