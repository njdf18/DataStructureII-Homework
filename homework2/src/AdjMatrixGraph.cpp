#include "AdjMatrixGraph.hpp"
#include <iostream>
#include <limits>

AdjMatrixGraph::AdjMatrixGraph(int n, bool d, bool w)
    : Graph(n, d, w), mat(n, std::vector<int>(n, 0)) {}

int AdjMatrixGraph::Degree(int u) const {
    int cnt = 0;
    for (int v = 0; v < n; v++)
        if (mat[u][v] != 0) cnt++;
    return cnt;
}

bool AdjMatrixGraph::ExistsEdge(int u, int v) const {
    return mat[u][v] != 0;
}

void AdjMatrixGraph::InsertVertex(int v) {
    n++;
    for (auto& row : mat) row.push_back(0);
    mat.push_back(std::vector<int>(n, 0));
}

void AdjMatrixGraph::InsertEdge(int u, int v, int w) {
    if (mat[u][v] == 0) {
        mat[u][v] = weighted ? w : 1;

        // 無向圖補對稱
        if (!directed) mat[v][u] = mat[u][v];

        e++;
    }
}

void AdjMatrixGraph::DeleteEdge(int u, int v) {
    if (mat[u][v] != 0) {
        mat[u][v] = 0;

        if (!directed) mat[v][u] = 0;

        e--;
    }
}

void AdjMatrixGraph::DeleteVertex(int v) {
    mat.erase(mat.begin() + v);
    for (auto& row : mat)
        row.erase(row.begin() + v);
    n--;
}

std::vector<Edge> AdjMatrixGraph::getNeighbors(int u) const {
    std::vector<Edge> res;

    for (int v = 0; v < n; v++)
        if (mat[u][v] != 0)
            res.push_back({ v, mat[u][v] });

    return res;
}

std::vector<std::tuple<int, int, int>> AdjMatrixGraph::getAllEdges() const {
    std::vector<std::tuple<int, int, int>> edges;

    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++) {
            if (mat[u][v] != 0) {
                if (directed || u < v)
                    edges.push_back({ u, v, mat[u][v] });
            }
        }
    }
    return edges;
}

void AdjMatrixGraph::Print() const {
    for (auto& row : mat) {
        for (auto x : row)
            std::cout << x << " ";
        std::cout << std::endl;
    }
}

void AdjMatrixGraph::ShortestPath(const int v) {
    s = std::vector<bool>(n, false);
    dist = std::vector<int>(n);
    constexpr int INF = std::numeric_limits<int>::max();

    for (int i = 0; i < n; i++) {
        if (mat[v][i] == 0 && i != v) {
            dist[i] = INF;
        } else {
            dist[i] = mat[v][i];
        }
    }
    s[v] = true;
    dist[v] = 0;

    for (int i = 0; i < n - 1; i++) {
        int u = Choose(n);
        if (u == -1) break;

        s[u] = true;
        for (int w = 0; w < n; w++) {
            if (!s[w] && mat[u][w] != 0) {
                if (dist[u] != INF && dist[u] + mat[u][w] < dist[w]) {
                    dist[w] = dist[u] + mat[u][w];
                }
            }
        }
    }
}

int AdjMatrixGraph::Choose(int n) {
    int u = -1;
    int min_dist = std::numeric_limits<int>::max();

    for (int i = 0; i < n; i++) {
        if (!s[i] && dist[i] < min_dist) {
            min_dist = dist[i];
            u = i;
        }
    }
    return u;
}

void AdjMatrixGraph::BellmanFord(const int v) {
    dist = std::vector<int>(n, std::numeric_limits<int>::max());
    dist[v] = 0;
    const int INF = std::numeric_limits<int>::max();

    for (int k = 1; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int u = 0; u < n; u++) {
                if (mat[i][u] != 0) { // Edge exists
                    if (dist[i] != INF && dist[i] + mat[i][u] < dist[u]) {
                        dist[u] = dist[i] + mat[i][u];
                    }
                }
            }
        }
    }
}

void AdjMatrixGraph::AllLengths() {
    auto a = std::vector<std::vector<int>>(n, std::vector(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = mat[i][j];
        }
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (a[i][k] + a[k][j] < a[i][j]) {
                    a[i][j] = a[i][k] + a[k][j];
                }
            }
        }
    }
}