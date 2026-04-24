#ifndef PRIM_HPP
#define PRIM_HPP

#include "Graph.hpp"
#include <vector>
#include <tuple>

class Prim
{
public:
    // 回傳 MST (邊的集合, 總權重)
    static std::pair<std::vector<std::tuple<int, int, int>>, int> findMST(const Graph& graph, int startNode = 0);
};

#endif // PRIM_HPP
