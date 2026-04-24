#include "Prim.hpp"
#include <queue>

using namespace std;

pair<vector<tuple<int, int, int>>, int> Prim::findMST(const Graph& graph, int startNode)
{
    int n = graph.NumberOfVertices();
    vector<tuple<int, int, int>> mstEdges;
    int totalCost = 0;

    if (n == 0 || startNode < 0 || startNode >= n) 
        return make_pair(mstEdges, 0);

    vector<bool> visited(n, false);
    
    typedef tuple<int, int, int> EdgeTuple;
    
    // Min-Heap 比較器
    struct CompareEdge
    {
        bool operator()(const EdgeTuple& a, const EdgeTuple& b)
        {
            return get<0>(a) > get<0>(b);
        }
    };
    
    priority_queue<EdgeTuple, vector<EdgeTuple>, CompareEdge> pq;

    visited[startNode] = true;
    
    vector<Edge> neighbors = graph.getNeighbors(startNode);
    for (size_t i = 0; i < neighbors.size(); i++)
    {
        pq.push(make_tuple(neighbors[i].weight, startNode, neighbors[i].to));
    }

    int edgeCount = 0;

    while (!pq.empty() && edgeCount < n - 1)
    {
        EdgeTuple topEdge = pq.top();
        pq.pop();
        
        int weight = get<0>(topEdge);
        int u = get<1>(topEdge);
        int v = get<2>(topEdge);

        if (visited[v]) continue;

        visited[v] = true;
        mstEdges.push_back(make_tuple(u, v, weight));
        totalCost += weight;
        edgeCount++;

        vector<Edge> nextNeighbors = graph.getNeighbors(v);
        for (size_t i = 0; i < nextNeighbors.size(); i++)
        {
            if (!visited[nextNeighbors[i].to])
            {
                pq.push(make_tuple(nextNeighbors[i].weight, v, nextNeighbors[i].to));
            }
        }
    }

    return make_pair(mstEdges, totalCost);
}
