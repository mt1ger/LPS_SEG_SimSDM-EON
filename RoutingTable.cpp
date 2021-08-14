// #define DEBUG_get_predecessor_list
// #define DEBUG_generate_routing_table
// #define DEBUG_get_shortest_path

#include "RoutingTable.h"
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

vector<vector<int>>
RoutingTable::single_src_routing_table(unsigned int src)
{
  stack<int>          pathStack;
  vector<int>         shortestPath;
  vector<vector<int>> singleSrcRoutingTable;

  for(unsigned int j = 0; j < network->numofNodes; j++)
  {
    if(j == src)
    {
      pathStack.push(-1);
    }
    else
      pathStack.push(j);
    unsigned int temp = j;
    while(temp != src)
    {
      pathStack.push(predecessors[src].at(temp));
      temp = predecessors[src].at(temp);
    }

    while(!pathStack.empty())
    {
      shortestPath.push_back(pathStack.top());
      pathStack.pop();
    }

    singleSrcRoutingTable.push_back(shortestPath);
    shortestPath.clear();
  }

  return singleSrcRoutingTable;
}

void
RoutingTable::get_predecessor_list()
{
  vector<int> hpredecessors;
  Topology    topology(network);
  Dijkstra    dijkstra(network);

  topology.read_topology();
  dijkstra.ajacent_nodes(dijkstra.ajacentNodes);

  for(unsigned int i = 0; i < network->numofNodes; i++)
  {
    dijkstra.shortest_path(i, -1, hpredecessors);
    predecessors.push_back(hpredecessors);
    hpredecessors.clear();
  }

#ifdef DEBUG_get_predecessor_list
  cout << endl;
  for(unsigned int i = 0; i < network->numofNodes; i++)
  {
    for(unsigned int j = 0; j < network->numofNodes; j++)
    {
      cout << ' ' << predecessors[i][j] + 1;
    }
    cout << endl;
  }
#endif
}

void
RoutingTable::generate_routing_table()
{
  get_predecessor_list();

  for(unsigned int i = 0; i < network->numofNodes; i++)
  {
    network->routingTable.push_back(single_src_routing_table(i));
  }

#ifdef DEBUG_generate_routing_table
  cout << endl;
  for(int i = 0; i < network->NumofNodes; i++)
  {
    cout << endl;
    cout << "Start to print table " << i << endl;
    for(int j = 0; j < network->NumofNodes; j++)
    {
      for(long unsigned int k = 0; k < network->routingTable[i][j].size(); k++)
      {
        cout << ' ' << network->routingTable[i][j][k] + 1;
      }
      cout << endl;
    }
  }
#endif
}

vector<int>
RoutingTable::get_shortest_path(int src, int dest)
{
  vector<int> shortestPath;

  for(long unsigned int i = 0; i < network->routingTable[src][dest].size(); i++)
  {
    shortestPath.push_back(network->routingTable[src][dest][i]);
  }

#ifdef DEBUG_get_shortest_path
  for(int i = 0; i < network->routingTable[src][dest].size(); i++)
  {
    cout << shortestPath[i] + 1 << ' ';
  }
#endif

  return shortestPath;
}
