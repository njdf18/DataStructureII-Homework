#include "AdjListArrayGraph.hpp"
#include <iostream>
#include <algorithm>

AdjListArrayGraph::AdjListArrayGraph(int n, bool d, bool w)
    : Graph(n, d, w), adj(n) {}

int AdjListArrayGraph::Degree(int u) const {
    return adj[u].size(); // 鄰點數
}

bool AdjListArrayGraph::ExistsEdge(int u, int v) const {
    for (auto& e : adj[u])
        if (e.to == v) return true;
    return false;
}

void AdjListArrayGraph::InsertVertex(int v) {
    adj.push_back({});
    n++;
}

void AdjListArrayGraph::InsertEdge(int u, int v, int w) {
    if (!ExistsEdge(u, v)) {
        adj[u].push_back({ v, weighted ? w : 1 });

        // 無向圖要補反向
        if (!directed)
            adj[v].push_back({ u, weighted ? w : 1 });

        e++;
    }
}

void AdjListArrayGraph::DeleteEdge(int u, int v) {
    if (!ExistsEdge(u, v)) return;

    for (auto it = adj[u].begin(); it != adj[u].end(); ++it)
        if (it->to == v) { adj[u].erase(it); break; }

    if (!directed) {
        for (auto it = adj[v].begin(); it != adj[v].end(); ++it)
            if (it->to == u) { adj[v].erase(it); break; }
    }
    e--;
}

void AdjListArrayGraph::DeleteVertex(int v) {
    // 把所有連到 v 的邊刪掉
    for (int i = 0; i < n; i++)
        if (i != v) DeleteEdge(i, v);

    adj.erase(adj.begin() + v);
    n--;
}

std::vector<Edge> AdjListArrayGraph::getNeighbors(int u) const {
    return adj[u];
}

std::vector<std::tuple<int, int, int>> AdjListArrayGraph::getAllEdges() const {
    std::vector<std::tuple<int, int, int>> edges;

    for (int u = 0; u < n; u++) {
        for (auto& e : adj[u]) {
            // 無向圖避免重複
            if (directed || u < e.to)
                edges.push_back({ u, e.to, e.weight });
        }
    }
    return edges;
}

void AdjListArrayGraph::Print() const {
    for (int i = 0; i < n; i++) {
        std::cout << i << ": ";
        for (auto& e : adj[i])
            std::cout << "(" << e.to << "," << e.weight << ") ";
        std::cout << std::endl;
    }
}
