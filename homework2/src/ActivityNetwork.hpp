#ifndef ACTIVITYNETWORK_HPP
#define ACTIVITYNETWORK_HPP

#include "Graph.hpp"
#include <vector>

// AOE
struct AOEActivity {
    int u;              //出發點
    int v;              //抵達點
    int weight;         //權重 (活動所需時間)
    int earlyTime;      //e(i)
    int lateTime;       //l(i)
    int slack;          //寬裕時間 l(i) - e(i)
    bool isCritical;    //是否為關鍵活動
};


class ActivityNetwork {
public:
    static std::vector<int> TopologicalOrder(const Graph& graph);
    //AOV: 回傳 Topological order

    static std::pair<std::vector<AOEActivity>, bool> AOENetwork(const Graph& graph);
    //AOE: 回傳是否成功
};

#endif // ACTIVITYNETWORK_HPP
