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
