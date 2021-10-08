#include "txtParse.h"
#include <iostream>
#include <string>
#define MU 0.2

#define LTDOFSEG 1

using namespace std;

int
main() {
  vector<int> Core, Lambda, Seed, Erlang;
  int         Number = 40;
  const string   parentPath = "/Users/mt1ger/Plot_Firstfit/";
  vector<string> filePaths;

  Core.push_back(1);
  /* Core.push_back(2); */
  /* Core.push_back(4); */
  Core.push_back(7);

  for(int i = 2; i < 143; i += 5) {
    Lambda.push_back(i);
  }
  for(int i = 0; i < Lambda.size(); i += 1) {
    Erlang.push_back(Lambda[i] / MU);
  }

  Seed.push_back(26851);
  Seed.push_back(16590);
  Seed.push_back(2893);
  Seed.push_back(25740);
  Seed.push_back(11179);

  for(unsigned int Max = 1; Max <= LTDOFSEG; Max *= 8) {
    string IsoFolder;

    for(unsigned int k = 0; k < Core.size(); k++) {
      IsoFolder
          = parentPath + to_string(Max) + "/Plot" + to_string(Core[k]) + '/';
      filePaths.push_back(IsoFolder);
    }
  }

  for(int i = 0; i < filePaths.size(); i++) {
    txtParse txtparse(Core, Erlang, Seed, filePaths[i]);
    txtparse.parse();
  }
}
