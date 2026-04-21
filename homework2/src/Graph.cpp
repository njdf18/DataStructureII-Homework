#include "Graph.hpp"

#include <iostream>
#include <ostream>
#include <queue>

void Graph::DFS() {
    visited = new bool[n];

    std::fill(visited, visited + n, false);
    DFSHelper(0);
    delete [] visited;
}

void Graph::DFSHelper(const int v) {
    visited[v] = true;

    const auto neighbors = getNeighbors(v);
    for (const auto edge : neighbors) {
        int w = edge.to;

        if (!visited[w]) {
            DFSHelper(w);
        }
    }
}

void Graph::BFS(int v) {
    visited = new bool[n];
    std::fill(visited, visited + n, false);

    visited[v] = true;
    std::queue<int> q;
    q.push(v);
    while (!q.empty()) {
        const int v2 = q.front();
        q.pop();

        const auto neighbors = getNeighbors(v2);
        for (auto edge : neighbors) {
            int w = edge.to;

            if (!visited[w]) {
                q.push(w);
                visited[w] = true;
            }
        }
    }
    delete [] visited;
}

void Graph::Components() {
    visited = new bool[n];
    std::fill(visited, visited + n, false);

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            DFSHelper(i);

            // outputs all vertices visited in the most recent invocation of DFS,
            // together with all edges incident on these vertices
            std::cout << "New Component: ";
            for (int j = 0; j < n; j++) {
                if (visited[j]) {
                    std::cout << j << std::endl;
                }
            }
        }
    }
    delete [] visited;
}

void Graph::DFNLow(const int x) {
    num = 1;
    dfn = new int[n];
    low = new int[n];
    std::fill(dfn, dfn + n, 0);
    std::fill(low, low + n, 0);

    DFNLowHelper(x, -1);
    delete [] dfn;
    delete [] low;
}

void Graph::DFNLowHelper(const int u, const int v) {
    dfn[u] = low[u] = num++;

    const auto neighbors = getNeighbors(u);
    for (auto edge : neighbors) {
        int w = edge.to;

        if (dfn[w]==0) {
            DFNLowHelper(w, u);
            low[u] = std::min(low[u], dfn[w]);
        }
        else if (w != v)
            low[u] = std::min(low[u], dfn[w]);
    }
}

void Graph::Biconnected() {
    num = 1;
    dfn = new int[n];
    low = new int[n];
    std::fill(dfn, dfn + n, 0);
    std::fill(low, low + n, 0);

    BiconnectedHelper(0, -1);
    delete [] dfn;
    delete [] low;
}

void Graph::BiconnectedHelper(const int u, const int v) {
    dfn[u] = low[u] = num++;

    const auto neighbors = getNeighbors(u);
    for (auto edge : neighbors) {
        int w = edge.to;

        if (v != w && dfn[w] < dfn[u]) {
            auto newEdge = Edge{u, v};
            edgeStack.push(newEdge);
        }
        if (dfn[w] == 0) {
            BiconnectedHelper(w, v);
            low[u] = std::min(low[u], low[w]);
            if (low[w] >= dfn[u]) {
                std::cout << "New Biconnected Component: " << std::endl;

                Edge topEdge{};
                do {
                    topEdge = edgeStack.top();
                    edgeStack.pop();
                    std::cout << topEdge.to << std::endl;

                } while (topEdge.to != w);  // (topEdge.x, topEdge.y)不該跟(u, w)相同
            }
        }
        else if (w != v)
            low[u] = std::min(low[u], dfn[w]);
    }
}