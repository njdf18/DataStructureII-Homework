#include "AdjMatrixGraph.hpp"
#include <iostream>

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
