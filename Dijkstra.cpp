#include "Dijkstra.h"
#include <cmath>
#include <iostream>
#include <vector>

#define EPSILON 0.00000001

using namespace std;

// Comparing numbers with double involved is trying to tell if the numbers are
// close enough.
int
compare_double(double a, double b)
{
  return abs(a - b) < EPSILON;
}

void
Dijkstra::ajacent_nodes(vector<vector<int>> &ajacentNodes)
{
  vector<int> ajecentNodes_sec;

  for(unsigned int i = 0; i < network->numNodes; i++)
  {
    for(unsigned int j = 0; j < network->numNodes; j++)
    {
      if(!compare_double(network->nodesWeight[i][j], -1) && (j != i))
        ajecentNodes_sec.push_back(j);
    }
    ajacentNodes.push_back(ajecentNodes_sec);
    ajecentNodes_sec.clear();
  }
}

void
Dijkstra::shortest_path(int src, int dest, vector<int> &predecessors)
{

  vector<double> dist;
  vector<bool>   visited;
  unsigned int   k;
  int            next;
  unsigned int   minDist;

  // To initialize the Algorithm:
  for(unsigned int i = 0; i < network->numNodes; i++)
  {
    dist.push_back(INFINITY);
    visited.push_back(false);
    predecessors.push_back(-1);
  }

  unsigned int counter = 0;
  dist[src]            = 0;
  visited[src]         = true;
  next                 = src;

  while(counter < network->numNodes)
  {
    for(long unsigned int j = 0; j < ajacentNodes[next].size(); j++)
    {
      if(!visited[ajacentNodes[next][j]]
         && ((dist[next] + network->nodesWeight[next][ajacentNodes[next][j]])
             < dist[ajacentNodes[next][j]]))
      {
        dist[ajacentNodes[next][j]]
            = (dist[next] + network->nodesWeight[next][ajacentNodes[next][j]]);
        predecessors.at(ajacentNodes[next][j]) = next;
      }
    }

    minDist = UINT32_MAX;
    for(k = 0; k < network->numNodes; k++)
    {
      if((minDist > dist[k]) && !visited[k])
      {
        minDist = dist[k];
        next    = k;
      }
    }
    visited[next] = true;

    if(next == dest)
    {
      break;
    }
    counter++;
  }
}
