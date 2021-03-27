#include "txtParse.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <sys/stat.h>
#include <thread>
#include <vector>

// #define NUMOFSEEDS 30
#define NUMOFSEEDS 3
// #define NUMOFREQUESTS 100000
#define NUMOFREQUESTS 1000
#define LTDOFSEG      4
// #define LTDOFSEG 1
#define MU 0.2

using namespace std;

void
thread_function(string cmd)
{
  system(cmd.c_str());
  // cout << cmd << endl;
}

int
main()
{
  // For FuFVFMs
  vector<int>         Core, Lambda, Seed, Erlang;
  double              Number;
  static unsigned int NumofRequests = NUMOFREQUESTS;
  static string Exec = "/Users/mt1ger/Code_Projects/Simulator/"
                       "LPS_SEG_SimSDM-EON/Sim_Firstfit";
  static string Topo = "/Users/mt1ger/Code_Projects/Simulator/"
                       "LPS_SEG_SimSDM-EON/Topology/NSF14.txt";
  static string Path = "/Users/mt1ger/Code_Projects/Simulator/"
                       "LPS_SEG_SimSDM-EON/Plot_Firstfit/";

  unsigned int   cnt = 0;
  string         Filename;
  txtParse       txtparse;
  int            NumofThreads = 0;
  vector<string> FolderSet;

  Core.push_back(1);
  Core.push_back(2);
  // Core.push_back(4);
  // Core.push_back(7);

  // for (int i = 100; i < 130; i += 10) {
  for(int i = 2; i < 101; i += 2)
  {
    Lambda.push_back(i);
  }

  for(int i = 0; i < Lambda.size(); i += 1)
  {
    Erlang.push_back(Lambda[i] / MU);
  }

  // cout << "Input a number for seed and press enter: " << endl;
  // cin >> Number;
  Number = 40;
  srand(Number);
  for(int i = 0; i < NUMOFSEEDS; i++)
  {
    Seed.push_back(rand() / 65535);
  }

  for(unsigned int Max = 1; Max <= LTDOFSEG; Max++)
  {
    NumofThreads = Core.size() * Lambda.size() * Seed.size();

    thread ThreadPointer[NumofThreads];
    string IsoFolder, Command, PlotFile;

    IsoFolder = Path + to_string(Max) + '/';
    FolderSet.push_back(IsoFolder);
    Command  = "mkdir " + IsoFolder;
    PlotFile = IsoFolder + "Plot.txt";
    system(Command.c_str());

    ofstream file(PlotFile);
    for(unsigned int k = 0; k < Core.size(); k++)
    {
      for(unsigned int lambda = 0; lambda < Lambda.size(); lambda++)
      {
        for(unsigned int seed = 0; seed < NUMOFSEEDS; seed++)
        {
          string Cmd = Exec + ' ' + Topo + ' ' + IsoFolder + ' '
                       + to_string(NumofRequests) + ' ' + to_string(Core[k])
                       + ' ' + to_string(Lambda[lambda] * Core[k]) + ' '
                       + to_string(MU) + ' ' + to_string(Seed[seed]) + ' '
                       + to_string(Max);
          ThreadPointer[cnt] = thread(thread_function, Cmd);
          cnt++;
          // thread_function(Cmd);
          // cout << Cmd << endl;
        }
      }
    }
    for(int i = 0; i < NumofThreads; i++)
    {
      ThreadPointer[i].join();
    }
    cnt = 0;
  }

  double Mu = MU;
  for(int i = 0; i < FolderSet.size(); i++)
  {
    // cout << FolderSet[i] << endl;
    txtparse.parse(Core, Erlang, Seed, Mu, FolderSet[i]);
  }

  return 1;
}
