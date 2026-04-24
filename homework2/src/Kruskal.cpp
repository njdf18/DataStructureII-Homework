#include "Kruskal.hpp"
#include <algorithm>
#include <vector>

using namespace std;

class DisjointSet
{
private:
    vector<int> parent;
public:
    DisjointSet(int n)
    {
        parent.assign(n, -1);
    }
    
    int Find(int i)
    {
        int root = i;
        while (parent[root] >= 0)
        {
            root = parent[root];
        }
        int curr = i;
        while (curr != root)
        {
            int nxt = parent[curr];
            parent[curr] = root;
            curr = nxt;
        }
        return root;
    }
    
    void Union(int i, int j)
    {
        int rootI = Find(i);
        int rootJ = Find(j);
        if (rootI != rootJ)
        {
            int temp = parent[rootI] + parent[rootJ];
            if (parent[rootI] < parent[rootJ])
            {
                parent[rootI] = temp;
                parent[rootJ] = rootI;
            }
            else
            {
                parent[rootJ] = temp;
                parent[rootI] = rootJ;
            }
        }
    }
};

struct CompareEdgeTuple
{
    bool operator()(const tuple<int, int, int>& a, const tuple<int, int, int>& b) const
    {
        return get<2>(a) < get<2>(b);
    }
};

pair<vector<tuple<int, int, int>>, int> Kruskal::findMST(const Graph& graph)
{
    int n = graph.NumberOfVertices();
    vector<tuple<int, int, int>> mstEdges;
    int totalCost = 0;

    if (n == 0) return make_pair(mstEdges, 0);

    vector<tuple<int, int, int>> allEdges = graph.getAllEdges();

    // 依權重由小到大排序
    sort(allEdges.begin(), allEdges.end(), CompareEdgeTuple());

    DisjointSet ds(n);
    int edgeCount = 0;

    for (size_t i = 0; i < allEdges.size(); i++)
    {
        int u = get<0>(allEdges[i]);
        int v = get<1>(allEdges[i]);
        int weight = get<2>(allEdges[i]);

        if (ds.Find(u) != ds.Find(v))
        {
            mstEdges.push_back(allEdges[i]);
            totalCost += weight;
            ds.Union(u, v);
            edgeCount++;
            if (edgeCount == n - 1) break;
        }
    }

    return make_pair(mstEdges, totalCost);
}
