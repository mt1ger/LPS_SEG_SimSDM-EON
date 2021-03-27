// #include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
// #include <sys/stat.h>
#include <thread>

using namespace std;

void
thread_func(string cmd)
{
  cout << cmd << endl;
  system(cmd.c_str());
}

int
main()
{
  vector<int> Core, Lambda, Seed, Erlang;

  static string Exec = "/Users/mt1ger/Code_Projects/Simulator/"
                       "LPS_SEG_SimSDM-EON/Sim_Firstfit";
  static string Topo = "/Users/mt1ger/Code_Projects/Simulator/"
                       "LPS_SEG_SimSDM-EON/Topology/NSF14.txt";
  // "LPS_SEG_SimSDM-EON/Topology/2node.txt";
  int  cnt           = 0;
  auto Num4Seed = 40;
  auto NumofRequests = 10000;
  // auto NumofSeeds    = 100;
  auto NumofSeeds    = 20;
  auto Max           = 1;
  auto MU            = 1;
  Core.push_back(1);
  Lambda.push_back(10000);

  srand(Num4Seed);
  for(int i = 0; i < NumofSeeds; i++)
  {
    Seed.push_back(rand() / 65535);
  }
  auto NumofThreads = Core.size() * Lambda.size() * Seed.size();
  thread threads[NumofThreads];
  for(unsigned int k = 0; k < Core.size(); k++)
  {
    for(unsigned int lambda = 0; lambda < Lambda.size(); lambda++)
    {
      for(unsigned int seed = 0; seed < NumofSeeds; seed++)
      {

        string Cmd = Exec + ' ' + Topo + ' ' + "./" + ' '
                     + to_string(NumofRequests) + ' ' + to_string(Core[k]) + ' '
                     + to_string(Lambda[lambda] * Core[k]) + ' ' + to_string(MU)
                     + ' ' + to_string(Seed[seed]) + ' ' + to_string(Max);
        thread_func(Cmd);
      }
    }
  }
}
