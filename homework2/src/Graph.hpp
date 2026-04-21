#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <stack>
#include <vector>
#include <tuple>

// ===============================
// Edge
// ===============================
struct Edge {
    int to;        // 連到哪個點
    int weight;    // 權重（沒設就是1）
};

// ===============================
// Graph 
// ===============================
class Graph {
protected:
    int n; // 節點數
    int e; // 邊數
    bool directed;  // 是否為有向圖
    bool weighted;  // 是否有權重

    bool* visited;  // 造訪過的節點(與Helper函式共用)
    int num = 0;  // 節點數量
    int* dfn;  // Depth-First Number
    int* low;  //
    std::stack<Edge> edgeStack;  //
    virtual void DFSHelper(const int v);
    virtual void DFNLowHelper(const int u, const int v);
    virtual void BiconnectedHelper(const int u, const int v);

public:
    Graph(int n = 0, bool d = false, bool w = false)
        : n(n), e(0), directed(d), weighted(w) {}

    virtual ~Graph() {}

    bool IsEmpty() const { return n == 0; }
    int NumberOfVertices() const { return n; }
    int NumberOfEdges() const { return e; }

    virtual int Degree(int u) const = 0;              // u 的鄰點數
    virtual bool ExistsEdge(int u, int v) const = 0;  // 是否有邊(u,v)

    virtual void InsertVertex(int v) = 0;             // 加點
    virtual void InsertEdge(int u, int v, int w = 1) = 0; // 加邊

    virtual void DeleteVertex(int v) = 0;             // 刪點
    virtual void DeleteEdge(int u, int v) = 0;        // 刪邊

    virtual std::vector<Edge> getNeighbors(int u) const = 0;
    // 取得 u 的所有鄰點

    virtual std::vector<std::tuple<int, int, int>> getAllEdges() const = 0;
    // 取得所有邊

    virtual void Print() const = 0;  // 印出圖

    virtual void DFS();  // Depth-First Search
    virtual void BFS(int v);  // Breadth-First Search
    virtual void Components();  // Determine the connected components of the graph.
    virtual void DFNLow(const int x);
    virtual void Biconnected();
};

#endif // GRAPH_HPP
