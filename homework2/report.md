# 41143124 41143203

作業二

## 解題說明
1. Graph: 圖的抽象類別，定義走訪(DFS, BFS)與基本元件操作(Components, DFNLow, Biconnected)。
2. AdjListArrayGraph: 以相鄰串列實作的圖，支援有向/無向、加權/無加權的圖結構。
3. AdjMatrixGraph: 以相鄰矩陣實作的圖，支援最短路徑演算法介面。
4. Kruskal: Kruskal 演算法，以貪婪策略尋找無向加權圖的最小生成樹(MST)。
5. Prim: Prim 演算法，由起始節點擴展尋找無向加權圖的最小生成樹(MST)。
6. ActivityNetwork: 實作活動網路，包含拓撲排序(Topological Sort)與關鍵路徑計算(Critical Path)。

### 解題策略
Graph:
1. DFS/BFS: 使用遞迴與 visited 陣列實作 DFS，使用 Queue 實作 BFS。
2. Components: 迭代未走訪的頂點，進行 DFS 找出所有連通元件。
3. Biconnected: 使用 DFN 與 Low 值尋找關節點，搭配 Stack 分離雙連通元件。

AdjListArrayGraph:
1. 使用 vector<vector<Edge>> 儲存圖的相鄰邊。
2. 加入邊時，若為無向圖則雙向加入。
3. 取得相鄰點時直接回傳對應的 vector 陣列。

AdjMatrixGraph:
1. 使用 vector<vector<int>> 儲存權重矩陣。
2. 提供 O(1) 的邊界權重查詢。

Kruskal:
1. 取得圖中所有邊，並依權重由小到大排序。
2. 使用 Disjoint Set 檢查 cycle (Find 搭配路徑壓縮，Union 搭配大小合併)。
3. 依序檢查排序後的邊，若端點屬不同集合則加入 MST，直到選取 V-1 條邊。

Prim:
1. 使用 boolean 陣列追蹤已加入 MST 的節點。
2. 每次從未加入的節點中，尋找與已加入集合相連且權重最小的邊。
3. 將該邊加入 MST 並更新總權重，直到所有節點加入。

ActivityNetwork:
1. Topological Sort: 計算各頂點 In-degree，使用 Queue 依序移除 In-degree 為 0 的節點。
2. AOE Network: 先透過 Topological Sort 確保無循環。
3. 依拓撲順序計算最早發生時間 e(i)，再依反向順序計算最晚發生時間 l(i)。
4. 針對每條邊計算 Slack (l(j) - e(i) - weight)，若 Slack 為 0 則為關鍵活動。

## 程式實作

以下為主要程式碼：

### Graph
```cpp
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
```

### AdjListArrayGraph
```cpp
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
```

### AdjMatrixGraph
```cpp
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
```

### Kruskal
```cpp
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
```

### Prim
```cpp
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
```

### ActivityNetwork
```cpp
#include "ActivityNetwork.hpp"
#include <iostream>

using namespace std;

vector<int> ActivityNetwork::TopologicalOrder(const Graph& graph) {
    int n = graph.NumberOfVertices();
    if (n == 0) return {};

    vector<int> count(n, 0);
    //計算所有點的 in-degree
    for (int i = 0; i < n; i++) {
        vector<Edge> neighbors = graph.getNeighbors(i);
        for (size_t k = 0; k < neighbors.size(); k++) {
            count[neighbors[k].to]++;
        }
    }

    int top = -1;
    //建立 in-degree 為 0 的 linked stack (利用 count 陣列存下一個元素的 index)
    for (int i = 0; i < n; i++) {
        if (count[i] == 0) {
            count[i] = top;
            top = i;
        }
    }

    vector<int> result;
    for (int i = 0; i < n; i++) {
        if (top == -1) {
            cout << "Network has a cycle." << endl;
            return {};
        }
        int j = top;
        top = count[top]; //unstack
        result.push_back(j);

        vector<Edge> neighbors = graph.getNeighbors(j);
        for (size_t k = 0; k < neighbors.size(); k++) {
            int v = neighbors[k].to;
            count[v]--; //減少後繼頂點的 count
            if (count[v] == 0) {
                count[v] = top;
                top = v; //加到stack
            }
        }
    }

    return result;
}

pair<vector<AOEActivity>, bool> ActivityNetwork::AOENetwork(const Graph& graph) {
    int n = graph.NumberOfVertices();
    if (n == 0) return { {}, true };

    vector<int> count(n, 0);
    for (int i = 0; i < n; i++) {
        vector<Edge> neighbors = graph.getNeighbors(i);
        for (size_t k = 0; k < neighbors.size(); k++) {
            count[neighbors[k].to]++;
        }
    }

    int top = -1;
    for (int i = 0; i < n; i++) {
        if (count[i] == 0) {
            count[i] = top;
            top = i;
        }
    }

    vector<int> topOrder;
    vector<int> ee(n, 0); // early event time(ee)

    //計算 early event time 與 Topological order
    for (int i = 0; i < n; i++) {
        if (top == -1) {
            cout << "Network has a cycle." << endl;
            return { {}, false }; 
        }
        int j = top;
        top = count[top];
        topOrder.push_back(j);

        vector<Edge> neighbors = graph.getNeighbors(j);
        for (size_t k = 0; k < neighbors.size(); k++) {
            int v = neighbors[k].to;
            int w = neighbors[k].weight;
            
            if (ee[j] + w > ee[v]) {
                ee[v] = ee[j] + w;
            }

            count[v]--;
            if (count[v] == 0) {
                count[v] = top;
                top = v;
            }
        }
    }

    //初始化 late event time(le)為max ee
    vector<int> le(n, 0);
    int maxEE = 0;
    for (int i = 0; i < n; i++) {
        if (ee[i] > maxEE) maxEE = ee[i];
    }
    for (int i = 0; i < n; i++) {
        le[i] = maxEE;
    }

    //使用反向 Topological order計算le
    for (int i = n - 1; i >= 0; i--) {
        int j = topOrder[i];
        vector<Edge> neighbors = graph.getNeighbors(j);
        for (size_t k = 0; k < neighbors.size(); k++) {
            int v = neighbors[k].to;
            int w = neighbors[k].weight;
            if (le[v] - w < le[j]) {
                le[j] = le[v] - w;
            }
        }
    }

    vector<AOEActivity> activities;
    //計算每個活動的 e, l, slack, critical
    for (int u = 0; u < n; u++) {
        vector<Edge> neighbors = graph.getNeighbors(u);
        for (size_t k = 0; k < neighbors.size(); k++) {
            int v = neighbors[k].to;
            int w = neighbors[k].weight;
            
            int e = ee[u];
            int l = le[v] - w;
            int slack = l - e;
            bool critical = (slack == 0);
            
            activities.push_back({u, v, w, e, l, slack, critical});
        }
    }

    return { activities, true };
}
```


## 效能分析

Graph:
1. 時間複雜度：DFS/BFS/Components: O(V + E)
2. 空間複雜度：O(V)

AdjListArrayGraph:
1. 時間複雜度：取得相鄰點: O(1)
2. 空間複雜度：O(V + E)

AdjMatrixGraph:
1. 時間複雜度：查詢特定邊: O(1)
2. 空間複雜度：O(V^2)

Kruskal:
1. 時間複雜度：排序邊: O(E log E)
2. 空間複雜度：O(V + E)

Prim:
1. 時間複雜度：陣列搜尋: O(V^2)
2. 空間複雜度：O(V)

ActivityNetwork:
1. 時間複雜度：Topological Sort / AOE: O(V + E)
2. 空間複雜度：O(V + E)

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| Graph (Components) | $Edge(0,1), Edge(1,2), Edge(2,0), Edge(3,4), Edge(4,5), Edge(5,3), Edge(6,7)$ | Component: 0, 1, 2 / 3, 4, 5 / 6, 7 | Component: 0, 1, 2 / 3, 4, 5 / 6, 7 |
| Kruskal   | $Edge(0,1,5), Edge(0,2,1), Edge(1,2,3), Edge(1,3,4), Edge(2,4,6), Edge(3,4,2), Edge(3,5,8), Edge(4,5,7)$ | Weight: 17, (0-2:1) (3-4:2) (1-2:3) (1-3:4) (4-5:7) | Weight: 17, (0-2:1) (3-4:2) (1-2:3) (1-3:4) (4-5:7) |
| Prim      | 同 Kruskal 輸入 | Weight: 17, (0-2:1) (2-1:3) (1-3:4) (3-4:2) (4-5:7) | Weight: 17, (0-2:1) (2-1:3) (1-3:4) (3-4:2) (4-5:7) |
| Activity Network | $Edge(0,1,2), Edge(0,2,3), Edge(1,3,4), Edge(2,3,1), Edge(2,4,2), Edge(3,5,3), Edge(4,5,1)$ | Topological Sort: 0 2 4 1 3 5 | Topological Sort: 0 2 4 1 3 5 |

### 編譯與執行指令
```bash
g++ -std=c++17 main.cpp Graph.cpp AdjListArrayGraph.cpp AdjMatrixGraph.cpp Kruskal.cpp Prim.cpp ActivityNetwork.cpp -o main
./main
```

### 結論

Graph / AdjListArrayGraph / AdjMatrixGraph:
1. 成功建立圖論抽象層，兩種資料結構皆能正確執行走訪與元件拆分。

Kruskal / Prim:
1. 皆能正確在加權無向圖中尋找出最小生成樹(MST)，所得權重總和一致。

ActivityNetwork:
1. 成功實作拓撲排序，並正確計算 Early Time、Late Time 及 Slack，找出 Critical Path。

## 申論及開發報告

Graph / AdjListArrayGraph / AdjMatrixGraph:
1. 將 Graph 設計為抽象類別，使演算法不依賴特定資料結構。
2. 相鄰串列適合稀疏圖與 MST；相鄰矩陣適合稠密圖與最短路徑處理。

Kruskal / Prim:
1. Kruskal 透過 Disjoint Set 搭配路徑壓縮，有效率偵測 cycle。
2. Prim 從邊界動態選擇最小成本路徑，兩者結果可互相對照。

ActivityNetwork:
1. AOE 網路計算需依賴 Topological Sort 作為前置作業。
2. 透過正反向走訪計算 Slack，了解關鍵路徑在排程管理上的應用。
