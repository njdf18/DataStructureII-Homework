#ifndef ADJLISTARRAYGRAPH_HPP
#define ADJLISTARRAYGRAPH_HPP

#include "Graph.hpp"
#include <vector>
#include <tuple>

class AdjListArrayGraph : public Graph {
private:
    std::vector<std::vector<Edge>> adj; // adjacency list（vector存）

public:
    AdjListArrayGraph(int n, bool d = false, bool w = false);

    int Degree(int u) const override;
    bool ExistsEdge(int u, int v) const override;
    void InsertVertex(int v) override;
    void InsertEdge(int u, int v, int w = 1) override;
    void DeleteEdge(int u, int v) override;
    void DeleteVertex(int v) override;
    std::vector<Edge> getNeighbors(int u) const override;
    std::vector<std::tuple<int, int, int>> getAllEdges() const override;
    void Print() const override;
};

#endif // ADJLISTARRAYGRAPH_HPP
